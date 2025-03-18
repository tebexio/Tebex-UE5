#pragma once

#include "CoreMinimal.h"
#include "DuePlayers.generated.h"

USTRUCT()
struct FDuePlayer
{
	GENERATED_BODY()

	UPROPERTY()
	int32 id;

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString uuid;

	FDuePlayer() : id(-1), name(""), uuid("") {}
};

USTRUCT()
struct FDuePlayersMeta
{
	GENERATED_BODY()

	UPROPERTY()
	bool execute_offline;

	UPROPERTY()
	int next_check;

	UPROPERTY()
	bool more;

	FDuePlayersMeta() : execute_offline(false), next_check(-1), more(false) {}
};



