#pragma once

#include "CoreMinimal.h"
#include "Result.generated.h"

USTRUCT()
struct FResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool success;

	UPROPERTY()
	FString message;

	FResult() : success(false), message(TEXT("")) {}
};

USTRUCT()
struct FErrorResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString Type;

	UPROPERTY()
	FString Title;

	UPROPERTY()
	int32 Status;
	
	UPROPERTY()
	FString Detail;
	
	UPROPERTY()
	FString Instance;

	FErrorResponse() : Type(TEXT("")), Title(TEXT("")), Status(0), Detail(TEXT("")), Instance(TEXT("")) {}
};