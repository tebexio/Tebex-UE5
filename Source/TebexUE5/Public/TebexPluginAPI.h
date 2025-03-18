#pragma once

#include "CoreMinimal.h"
#include "Util/SharedAliases.h"

#include "Headless/EmptyBody.h"
#include "Plugin/ResponseTypes.h"
#include "Plugin/Models/JoinEvent.h"
#include "Plugin/Models/ServerInformation.h"
#include "TebexPluginAPI.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTebexPlugin, Log, All);

UCLASS()
class UTebexPluginAPI : public UObject
{
	GENERATED_BODY()
public:
	static const FString BaseUrl;

	static void GetServerInformation(TOkCallable<FServerInformation> Callback, FErrCallable Error);
	static void GetDuePlayers(TOkCallable<FDuePlayersResponse> Callback, FErrCallable Error);
	static void GetOfflineCommands(TOkCallable<FOfflineCommandsResponse> Callback, FErrCallable Error);
	static void GetOnlineCommands(const FString& PlayerId, TOkCallable<FOnlineCommandsResponse> Callback, FErrCallable Error);
	static void DeleteCommands(const TArray<int>& CommandIds, TOkCallable<FEmptyBody> Callback, FErrCallable Error);
	static void CreateCheckoutUrl(int PackageId, const FString& Username, TOkCallable<FCheckoutResponse> Callback, FErrCallable Error);
	static void ListActivePackages(const FString& PlayerId, TOkCallable<FActivePackagesResponse> Callback, FErrCallable Error);
	static void SendEvents(const FJoinEvents& Events, TOkCallable<FEmptyBody> Callback, FErrCallable Error);
	static void SetSecretKey(FString Key);
private:
	static FString SecretKey;
	static bool SecretKeyIsSet;

	template <typename ReturnType, typename RequestBodyType>
	static void MakeRequest(const FString& Url, const FString& Verb, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable Error);

	template <typename ReturnType, typename RequestBodyType>
	static void MakeRequest(const FString& Url, const FString& Verb, TArray<RequestBodyType> DataArray, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable ErrorCallback);
	
	template <typename ReturnType, typename RequestBodyType>
	static void MakeRequest(const FString& Url, const FString& Verb, RequestBodyType SerializableRequestBody, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable Error);
};