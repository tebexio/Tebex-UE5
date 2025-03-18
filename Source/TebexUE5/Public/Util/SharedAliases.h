#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"

// Callback type for handling successful results of async operations
// TSuccessReturnType - The type of the result expected from the API
template <typename TSuccessReturnType>
using TOkCallable = const TFunction<void(const TSuccessReturnType&)>&;

// Callback type for handling errors during asynchronous operations.
// Takes an FHttpResponsePtr (shared pointer to an HTTP response)
using FErrCallable = const TFunction<void(FHttpResponsePtr)>&;

// Represents a shared pointer to an HTTP response
// Used to pass error responses back to the caller
using FHttpErr = const FHttpResponsePtr&;