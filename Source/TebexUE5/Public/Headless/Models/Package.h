#pragma once

#include "CoreMinimal.h"
#include "Package.generated.h"

USTRUCT()
struct FPackageCategory
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Id;

	UPROPERTY()
	FString Name;
	
	FPackageCategory(): Id(0), Name(TEXT("")) {}
};

/**
 * @struct FTebexPackage
 * 
 * @brief Represents a Package object in the Headless API.
 *
 * Encapsulates detailed purchasable package information including metadata, pricing, and categorization. Packages are defined
 * by creators in their webstores.
 */
USTRUCT()
struct FTebexPackage
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Id;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString Type;

	UPROPERTY()
	FPackageCategory Category;

	UPROPERTY()
	float Base_Price;

	UPROPERTY()
	float Sales_Tax;

	UPROPERTY()
	float Total_Price;

	UPROPERTY()
	FString Currency;

	UPROPERTY()
	float Discount;

	UPROPERTY()
	bool Disable_Quantity;

	UPROPERTY()
	bool Disable_Gifting;

	UPROPERTY()
	FString Created_At;

	UPROPERTY()
	FString Updated_At;

	UPROPERTY()
	int32 Order;

	FTebexPackage() :
		Id(0), Name(TEXT("")), Description(TEXT("")), Type(TEXT("")),
		Category(FPackageCategory()), Base_Price(-1.0f), Sales_Tax(-1.0f), Total_Price(-1.0f), Currency(TEXT("")),
		Discount(-1.0f), Disable_Quantity(false), Disable_Gifting(false), Created_At(TEXT("")),
		Updated_At(TEXT("")), Order(-1) {}
};

USTRUCT()
struct FAddPackagePayload
{
	GENERATED_BODY()

	UPROPERTY()
	int32 package_id;

	UPROPERTY()
	int32 quantity;
	
	UPROPERTY()
	TMap<FString, FString> variable_data;

	FAddPackagePayload(): package_id(-1), quantity(-1), variable_data() {}
};

USTRUCT()
struct FPackageQuantity
{
	GENERATED_BODY()
	UPROPERTY()
	int32 Quantity;

	FPackageQuantity(): Quantity(-1) {}

	FPackageQuantity(int32 quantity) : Quantity(quantity) {}
};

USTRUCT()
struct FPackageRemovePayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString package_id; // numeric package ID provided when removing a package from the basket

	FPackageRemovePayload(): package_id(TEXT("")) {}
};

USTRUCT()
struct FPackages
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FTebexPackage> packages;

	FPackages(): packages() {}
};

USTRUCT()
struct FWrappedPackage
{
	GENERATED_BODY()
	UPROPERTY()
	FTebexPackage data;

	FWrappedPackage(): data() {}
};

USTRUCT()
struct FWrappedPackages
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FWrappedPackage> packages;

	FWrappedPackages(): packages() {}
};

USTRUCT()
struct FPackagesResponse
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FTebexPackage> data;

	FPackagesResponse(): data() {}
};
