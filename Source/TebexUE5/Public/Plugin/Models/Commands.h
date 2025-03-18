#pragma once

#include "CoreMinimal.h"
#include "Commands.generated.h"

USTRUCT()
struct FOfflineCommandsMeta
{
	GENERATED_BODY()

	UPROPERTY()
	bool limited;

	FOfflineCommandsMeta(): limited(false) {}
};

USTRUCT()
struct FCommandConditions
{
	GENERATED_BODY()

	UPROPERTY()
	int32 delay;

	UPROPERTY()
	int32 slots;

	FCommandConditions() : delay(0), slots(0) {}
};

USTRUCT()
struct FCommand
{
	GENERATED_BODY()

	UPROPERTY()
	int32 id;

	UPROPERTY()
	FString command;

	UPROPERTY()
	int32 payment;

	UPROPERTY()
	int32 package;

	UPROPERTY()
	FCommandConditions conditions;

	FCommand() : id(-1), command(""), payment(0), package(0) {}
};