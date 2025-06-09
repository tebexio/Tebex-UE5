#pragma once

USTRUCT()
struct FTierStatus
{
	GENERATED_BODY()

	UPROPERTY()
	int id;

	UPROPERTY()
	FString name;

	FTierStatus() : id(-1), name("") {}
};

USTRUCT()
struct FTierPendingDowngradePackage
{
	GENERATED_BODY()

	UPROPERTY()
	int id;

	UPROPERTY()
	FString name;

	FTierPendingDowngradePackage() : id(-1), name("") {}
};

USTRUCT()
struct FUpdateTierPayload
{
	GENERATED_BODY()

	UPROPERTY()
	int package_id;

	FUpdateTierPayload() : package_id(-1) {}
};

USTRUCT()
struct FUpdateTierResponse
{
	GENERATED_BODY()

	UPROPERTY()
	bool success;

	UPROPERTY()
	FString message;

	FUpdateTierResponse() : success(false), message(TEXT("")) {}
};

USTRUCT()
struct FTier
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Id;

	UPROPERTY()
	FDateTime created_at;

	UPROPERTY()
	FString username_id;

	UPROPERTY()
	FTebexPackage tier_package;

	UPROPERTY()
	bool active;

	UPROPERTY()
	FString recurring_payment_reference;

	UPROPERTY()
	FDateTime next_payment_date;

	UPROPERTY()
	FTierStatus status;

	UPROPERTY()
	FTierPendingDowngradePackage pending_downgrade_package;

	FTier() : Id(-1), created_at(FDateTime::UtcNow()), username_id(""), tier_package(), active(false), recurring_payment_reference(""), next_payment_date(), status(), pending_downgrade_package() {}
};