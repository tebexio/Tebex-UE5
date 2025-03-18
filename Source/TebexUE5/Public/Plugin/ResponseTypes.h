#pragma once

#include "CoreMinimal.h"
#include "Models/Commands.h"
#include "Models/DuePlayers.h"
#include "ResponseTypes.generated.h"

USTRUCT()
struct FOnlineCommandsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FCommand> commands;
};

USTRUCT()
struct FOfflineCommandsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FOfflineCommandsMeta meta;

	UPROPERTY()
	TArray<FCommand> commands;
	
	FOfflineCommandsResponse(): meta(), commands() {}
};

USTRUCT()
struct FDuePlayersResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FDuePlayersMeta meta;

	UPROPERTY()
	TArray<FDuePlayer> players;

	FDuePlayersResponse(): meta(), players() {}
};

USTRUCT()
struct FCheckoutResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString url;

	UPROPERTY()
	FString expires;

	FCheckoutResponse() : url(""), expires("") {}
};

USTRUCT()
struct FActivePackageRecord
{
	GENERATED_BODY()

	UPROPERTY()
	int32 id;

	UPROPERTY()
	FString name;

	FActivePackageRecord() : id(-1), name(TEXT("")) {}
};

USTRUCT()
struct FActivePackagesResponseObject
{
	GENERATED_BODY()

	UPROPERTY()
	FString txn_id;

	UPROPERTY()
	FString date;

	UPROPERTY()
	int32 quantity;

	UPROPERTY()
	FActivePackageRecord package;

	FActivePackagesResponseObject() : txn_id(""), date(""), quantity(-1), package() {}
};

USTRUCT()
struct FActivePackagesResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FActivePackagesResponseObject> packages;

	FActivePackagesResponse() : packages() {}
};