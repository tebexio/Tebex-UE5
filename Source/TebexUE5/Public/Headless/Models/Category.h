#pragma once

#include "CoreMinimal.h"
#include "Package.h"
#include "Tiers.h"
#include "Category.generated.h"

USTRUCT()
struct FCategory
{
	GENERATED_BODY()

	UPROPERTY()
	int32 id;

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString slug;

	UPROPERTY()
	FString description;

	UPROPERTY()
	FPackageCategory parent;

	UPROPERTY()
	int32 order;

	UPROPERTY()
	FString display_type;

	UPROPERTY()
	TArray<FTebexPackage> packages;

	UPROPERTY()
	bool tiered;

	UPROPERTY()
	FTier active_tier;
	
	FCategory(): id(-1), name(TEXT("")), description(TEXT("")), order(-1), display_type(TEXT("")), tiered(false), active_tier() {}
};

USTRUCT()
struct FWrappedCategory
{
	GENERATED_BODY()
	UPROPERTY()
	FCategory data;

	FWrappedCategory(): data(FCategory()) {}
};

USTRUCT()
struct FWrappedCategories
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FWrappedCategory> categories;

	FWrappedCategories(): categories(TArray<FWrappedCategory>()) {}
};

USTRUCT()
struct FCategoriesResponse
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FCategory> data;

	FCategoriesResponse(): data(TArray<FCategory>()) {}
};