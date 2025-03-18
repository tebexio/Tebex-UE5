#pragma once

#include "CoreMinimal.h"
#include "Promotions.generated.h"

USTRUCT()
struct FCreatorCode
{
	GENERATED_BODY()

	UPROPERTY()
	FString creator_code;

	FCreatorCode() : creator_code("") {}

	FCreatorCode(const FString& creator_code) : creator_code(creator_code) {}
};

USTRUCT()
struct FGiftCardNumber
{
	GENERATED_BODY()

	UPROPERTY()
	FString Card_Number;

	FGiftCardNumber() : Card_Number("") {}
};