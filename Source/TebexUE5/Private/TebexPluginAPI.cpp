#include "TebexPluginAPI.h"

#include "Util/SharedAliases.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "TebexUE5.h"
#include "Headless/EmptyBody.h"
#include "Plugin/RequestTypes.h"
#include "Plugin/Models/JoinEvent.h"

const FString UTebexPluginAPI::BaseUrl = "https://plugin.tebex.io/";
FString UTebexPluginAPI::SecretKey = TEXT("");
bool UTebexPluginAPI::SecretKeyIsSet = false;

DEFINE_LOG_CATEGORY(LogTebexPlugin);

void UTebexPluginAPI::SetSecretKey(FString Key)
{
	if (Key.IsEmpty())
	{
		FTebexUE5::Log("Secret key is empty.");
		return;
	}

	SecretKey = Key;
	SecretKeyIsSet = true;
}

void UTebexPluginAPI::GetServerInformation(TOkCallable<FServerInformation> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "information";
	MakeRequest<FServerInformation, FEmptyBody>(Url, "GET", Callback, Error);
}

void UTebexPluginAPI::GetDuePlayers(TOkCallable<FDuePlayersResponse> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "queue";
	MakeRequest<FDuePlayersResponse, FEmptyBody>(Url, "GET", Callback, Error);
}

void UTebexPluginAPI::GetOfflineCommands(TOkCallable<FOfflineCommandsResponse> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "queue/offline-commands";
	MakeRequest<FOfflineCommandsResponse, FEmptyBody>(Url, "GET", Callback, Error);
}

void UTebexPluginAPI::GetOnlineCommands(const FString& PlayerId, TOkCallable<FOnlineCommandsResponse> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "queue/online-commands/" + PlayerId;
	MakeRequest<FOnlineCommandsResponse, FEmptyBody>(Url, "GET", Callback, Error);
}

void UTebexPluginAPI::DeleteCommands(const TArray<int>& CommandIds, TOkCallable<FEmptyBody> Callback, FErrCallable Error)
{
	FDeleteCommandsRequest DeleteCommandsRequest = FDeleteCommandsRequest(CommandIds);
	const FString Url = BaseUrl + "queue";
	MakeRequest<FEmptyBody, FDeleteCommandsRequest>(Url, "DELETE", DeleteCommandsRequest, Callback, Error);
}

void UTebexPluginAPI::CreateCheckoutUrl(const int PackageId, const FString& Username, TOkCallable<FCheckoutResponse> Callback, FErrCallable Error)
{
	FPluginCheckoutRequest Request = FPluginCheckoutRequest(FString::FromInt(PackageId), Username); 
	const FString Url = BaseUrl + "checkout";
	MakeRequest<FCheckoutResponse, FPluginCheckoutRequest>(Url, "POST", Request, Callback, Error);
}

void UTebexPluginAPI::ListActivePackages(const FString& PlayerId, TOkCallable<FActivePackagesResponse> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "player/" + PlayerId + "/packages";
	MakeRequest<FActivePackagesResponse, FEmptyBody>(Url, "GET", Callback, Error);
}

void UTebexPluginAPI::SendEvents(const FJoinEvents& Events, TOkCallable<FEmptyBody> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "events";
	MakeRequest<FEmptyBody, FJoinEvent>(Url, "POST", Events.events, Callback, Error);
}

template <typename ReturnType, typename RequestBodyType>
void UTebexPluginAPI::MakeRequest(const FString& Url, const FString& Verb, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable Error)
{
	MakeRequest<ReturnType, RequestBodyType>(Url, Verb, FTebexUE5::EmptyRequest, OnSuccess, Error);	
}

template <typename ReturnType, typename RequestBodyType>
void UTebexPluginAPI::MakeRequest(const FString& Url, const FString& Verb, TArray<RequestBodyType> DataArray, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable ErrorCallback)
{
    if (!SecretKeyIsSet) // Require the secret key to be set
    {
        FTebexUE5::Log("Plugin secret key is not set.");
        return;
    }

    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) // Ensure the HTTP module is available
    {
        FTebexUE5::Log("HTTP Module not initialized!");
        return;
    }

    // Build the request
    const TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(Verb);
    Request->SetHeader("X-Tebex-Secret", SecretKey);
    Request->SetHeader("Content-Type", "application/json");

    // Serialize the TArray of objects into a JSON array string
    FString RequestBodyJson = TEXT("");
    TArray<TSharedPtr<FJsonValue>> JsonArray;

    for (const RequestBodyType& Object : DataArray)
    {
        // Use FJsonObjectConverter to serialize each object into a JSON Object
        TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
        if (FJsonObjectConverter::UStructToJsonObject(RequestBodyType::StaticStruct(), &Object, JsonObject.ToSharedRef(), 0, 0))
        {
            JsonArray.Add(MakeShared<FJsonValueObject>(JsonObject));
        }
        else
        {
            FTebexUE5::Log("Failed to serialize an object in the array.");
            return;
        }
    }

    // Convert the array of JSON Objects to a JSON string
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBodyJson);
    if (FJsonSerializer::Serialize(JsonArray, Writer))
    {
        Request->SetContentAsString(RequestBodyJson);
    }
    else
    {
        FTebexUE5::Log("Failed to serialize the JSON array.");
        return;
    }

    // Log the request details
    FTebexUE5::Log("-> " + Verb + " " + Url);
    if (RequestBodyJson.Len() > 0)
    {
        FTebexUE5::Log("  with body: " + RequestBodyJson);
    }

    // Bind to the response handler, parse and handle the HTTP response
    Request->OnProcessRequestComplete().BindLambda(
        [Verb, OnSuccess, ErrorCallback](const FHttpRequestPtr& CompletedRequest, const FHttpResponsePtr& Response, const bool bWasSuccessful)
        {
            const FString ResponseBody = Response->GetContentAsString();
            const int ResponseCode = Response->GetResponseCode();

            FTebexUE5::Log("<- " + FString::FromInt(ResponseCode) + " " + Verb + " " + CompletedRequest->GetURL() + " with body: " + ResponseBody);

            if (!bWasSuccessful)
            {
                if (ErrorCallback)
                {
                    ErrorCallback(Response);
                }
                else
                {
                    FTebexUE5::Log("Request failed without error handler!");
                }
                return;
            }

            if (ResponseCode >= 200 && ResponseCode <= 204)
            {
            	if (ResponseCode == 204) // Handle deserialization of no-content from submitting events
            	{
					ReturnType EmptyResponse;
					OnSuccess(EmptyResponse);
					return;
				}
            	
                // On success, parse the response into the expected return type
                ReturnType ParsedResponse;
                if (FJsonObjectConverter::JsonObjectStringToUStruct<ReturnType>(
                        ResponseBody,
                        &ParsedResponse,
                        0, // No flags
                        0)) // No CheckFlags
                {
                    if (OnSuccess)
                    {
                        PrintUStructProperties(ParsedResponse);
                        OnSuccess(ParsedResponse);
                    }
                }
                else
                {
                    FTebexUE5::Log("Failed to parse response body as JSON.");
                    if (ErrorCallback)
                    {
                        ErrorCallback(Response);
                    }
                }
            }
            else
            {
                if (ErrorCallback)
                {
                    ErrorCallback(Response);
                }
            }
        });

    // Process the HTTP request (async)
    if (!Request->ProcessRequest())
    {
        FTebexUE5::Log("Request failed to process!");
    }
}

template <typename ReturnType, typename RequestBodyType>
void UTebexPluginAPI::MakeRequest(const FString& Url, const FString& Verb, RequestBodyType SerializableRequestBody, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable OnError)
{
	if (!SecretKeyIsSet) // Require a store's secret key to be set for all requests.
	{
		FTebexUE5::Log("Plugin secret key is not set.");
		return;
	}

	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) // Ensure we have HTTP available (we're not in the middle of shutdown / http is not included)
 	{
		FTebexUE5::Log("HTTP Module not initialized!");
		return;
	}

	// Build the request
	const TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader("X-Tebex-Secret", SecretKey);
	Request->SetHeader("Content-Type", "application/json");

	// If data is provided in request body, serialize the incoming request body as JSON and set the request content
	FString RequestBodyJson = TEXT("");
	if (!std::is_same_v<RequestBodyType, FEmptyBody>)
	{
		if (FJsonObjectConverter::UStructToJsonObjectString(RequestBodyType::StaticStruct(), &SerializableRequestBody, RequestBodyJson))
		{
			Request->SetContentAsString(RequestBodyJson);
		}
		else
		{
			FTebexUE5::Log("Failed to serialize request body: " + RequestBodyJson);
			return;
		}
	}

	// Log the request being sent
	FTebexUE5::Log("-> " + Verb + " " + Url);
	if (RequestBodyJson.Len() > 0)
	{
		FTebexUE5::Log("  with body: " + RequestBodyJson);	
	}
	
	// Bind a function to consume the response. All responses are expected to be JSON, so we will parse the response
	// into the expected StructType template provided with the request call.
	Request->OnProcessRequestComplete().BindLambda(
		[Verb, OnSuccess, OnError](const FHttpRequestPtr& CompletedRequest, const FHttpResponsePtr& Response, const bool bWasSuccessful)
		{
			const FString ResponseBody = Response->GetContentAsString();
			const int ResponseCode = Response->GetResponseCode();

			FTebexUE5::Log("<- " + FString::FromInt(ResponseCode) + " " + Verb + " " + CompletedRequest->GetURL() + " with body: " + ResponseBody);
			
			if (!bWasSuccessful)
			{
				if (OnError)
				{
					OnError(Response);
				} else
				{
					FTebexUE5::Log("Request failed without error handler!");
				}
				return;
			}
			
			if (ResponseCode >= 200 && ResponseCode <= 204)
			{
				if (ResponseCode == 204) // No content from submitting events
				{
					ReturnType EmptyResponse;
					OnSuccess(EmptyResponse);
					return;
				}
				
				// On success, parse the JSON response into the expected return type
				ReturnType ParsedResponse;
				if (FJsonObjectConverter::JsonObjectStringToUStruct<ReturnType>(
					   ResponseBody,
					   &ParsedResponse,
					   0, // No flags
					   0)) // No CheckFlags
				{
					if (OnSuccess)
					{
						PrintUStructProperties(ParsedResponse);
						OnSuccess(ParsedResponse); // Pass parsed struct to the callback func
					}
				}
				else // Fails to parse response into JSON
				{
					FTebexUE5::Log("Failed to parse response body as JSON.");
					if (OnError)
					{
						OnError(Response);	
					}					
				}
			}
			else // Non-success response code
			{
				if (OnError)
				{
					OnError(Response);
				}
			}
		});

	// Executes the HTTP request. This is async, which returns when the request is successfully started.
	if (!Request->ProcessRequest())
	{
		FTebexUE5::Log("Request failed to process!");
	}
}