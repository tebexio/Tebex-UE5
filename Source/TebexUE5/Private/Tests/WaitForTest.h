#pragma once

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "TebexUE5.h"

template <typename T>
/**
 * FWaitForTest allows asynchronous testing operations and validation in Unreal's automation testing environment.
 *
 * @tparam T Type of data returned by the asynchronous operation.
 */
class FWaitForTest : public IAutomationLatentCommand
{
public:
    FAutomationTestBase* Test; // Pointer to the current test instance
    T ResponseData;            // Holds the response data
    bool bResponseReceived = false; // Flag to indicate completion
    bool bSuccess = false;        // Flag to indicate success of the http request
    FHttpResponsePtr ErrorResponsePtr; // Holds the error response if any
    TFunction<void(TFunction<void(T&)>, TFunction<void(FHttpResponsePtr&)>)> AsyncOperation; // The async operation taking a callback for the success type and an error type for the response
    TFunction<void(FAutomationTestBase*, const T&)> Validation; // Validation function where tests are run
    
    FWaitForTest(
        FAutomationTestBase* InTest,
        TFunction<void(TFunction<void(T&)>, TFunction<void(FHttpResponsePtr&)>)> InAsyncOperation,
        TFunction<void(FAutomationTestBase*, const T&)> InValidation)
        : Test(InTest), AsyncOperation(InAsyncOperation), Validation(InValidation)
    {
        // Start the async request
        AsyncOperation([this](T& Data)
        {
            // When we reach this, we're in the callback provided to AsyncOperation
            ResponseData = Data;
            bSuccess = true; // Signal the response succeeded
            bResponseReceived = true; // Signal that we finished the response
        }, [this](const FHttpResponsePtr& ErrorResponse){
            ErrorResponsePtr = ErrorResponse;
            bSuccess = false;
            bResponseReceived = true;
        });
    }
    
    virtual bool Update() override
    {
        if (bResponseReceived) // When we see response is done, perform validation.
        {
            if (!bSuccess)
            {
                FTebexUE5::Log("Request failed.");
                FTebexUE5::Log("Response code: " + FString::FromInt(ErrorResponsePtr->GetResponseCode()));
                FTebexUE5::Log("Response body: " + ErrorResponsePtr->GetContentAsString());
                Test->TestFalse("Request failed", true);
                return true; // stop waiting and don't trigger validation of the response data
            }
            
            Validation(Test, ResponseData);
            return true; // Test completes successfully
        }

        return false; // False indicates to continue waiting
    }
};