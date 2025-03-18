#pragma once

#include "CoreMinimal.h"
#include "Webstore.generated.h"

USTRUCT()
struct FWebstore
{
	GENERATED_BODY()

	UPROPERTY()
	int ID;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Webstore_Url;

	UPROPERTY()
	FString Currency;

	UPROPERTY()
	FString Lang;

	UPROPERTY()
	FString Logo;

	UPROPERTY()
	FString Platform_Type;

	UPROPERTY()
	FString Platform_Type_Id;

	UPROPERTY()
	FString Created_At;

	FWebstore() : ID(-1), Description(), Name(), Webstore_Url(), Currency(), Lang(), Logo(), Platform_Type(), Platform_Type_Id(), Created_At() {}
};

USTRUCT()
struct FWebstoreResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FWebstore Data;
};