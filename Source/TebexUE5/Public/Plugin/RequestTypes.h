#pragma once

#include "CoreMinimal.h"
#include "RequestTypes.generated.h"

USTRUCT()
struct FPluginCheckoutRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString package_id;

	UPROPERTY()
	FString username;

	FPluginCheckoutRequest(): package_id(""), username("") {}

	FPluginCheckoutRequest(const FString& Package_Id, const FString& Username): package_id(Package_Id), username(Username) {}
};

USTRUCT()
struct FDeleteCommandsRequest
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int32> ids;

	FDeleteCommandsRequest() : ids() {}

	FDeleteCommandsRequest(const TArray<int32>& command_ids) : ids(command_ids) {}
};