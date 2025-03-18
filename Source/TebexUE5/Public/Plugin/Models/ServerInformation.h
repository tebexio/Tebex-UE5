#pragma once

#include "CoreMinimal.h"
#include "ServerInformation.generated.h"

USTRUCT()
struct FServerCurrency
{
	GENERATED_BODY()

	UPROPERTY()
	FString iso_4217;

	UPROPERTY()
	FString symbol;

	FServerCurrency(): iso_4217(TEXT("")), symbol(TEXT("")) {}
};

USTRUCT()
struct FAccount
{
	GENERATED_BODY()

	UPROPERTY()
	int32 id;

	UPROPERTY()
	FString domain;

	UPROPERTY()
	FString name;

	UPROPERTY()
	FServerCurrency currency;

	UPROPERTY()
	bool online_mode;

	UPROPERTY()
	FString game_type;

	UPROPERTY()
	bool log_events;

	FAccount(): id(-1), domain(TEXT("")), name(TEXT("")), currency(), online_mode(false), game_type(TEXT("")), log_events(false) {}
};

USTRUCT()
struct FServer
{
	GENERATED_BODY()

	UPROPERTY()
	int32 id;

	UPROPERTY()
	FString name;

	FServer(): id(-1), name(TEXT("")) {}
};

USTRUCT()
struct FServerInformation
{
	GENERATED_BODY()
	
	UPROPERTY()
	FAccount account;

	UPROPERTY()
	FServer server;

	FServerInformation(): account(), server() {}
};