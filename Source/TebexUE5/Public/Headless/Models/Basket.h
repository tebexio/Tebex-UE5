#pragma once

#include "CoreMinimal.h"
#include "Basket.generated.h"

USTRUCT()
struct FCreateBasketPayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString email;
	
	UPROPERTY()
	FString username; // some UUID, player ID, or otherwise unique identifier for the user
	
	UPROPERTY()
	FString cancel_url; // where the user goes if cancelling the transaction

	UPROPERTY()
	FString complete_url; // where the user goes if transaction succeeds

	UPROPERTY()
	TMap<FString, FString> custom; //see api reference for list of additional fields that can be used here

	UPROPERTY()
	bool complete_auto_redirect;
	
	FCreateBasketPayload(): email(TEXT("")), username(TEXT("")), cancel_url(TEXT("")), complete_url(TEXT("")), custom(), complete_auto_redirect(false)
	{
	}
};

USTRUCT()
struct FBasketRevenueShare
{
	GENERATED_BODY()

	UPROPERTY()
	FString wallet_ref;

	UPROPERTY()
	float amount;

	UPROPERTY()
	int32 gateway_fee_percent;

	FBasketRevenueShare() : wallet_ref(TEXT("")), amount(-1.0f), gateway_fee_percent(-1){}
};

USTRUCT()
struct FInnerBasketPackage
{
	GENERATED_BODY()

	UPROPERTY()
	int32 quantity;

	UPROPERTY()
	float price;

	UPROPERTY()
	FString gift_username_id;

	UPROPERTY()
	FString gift_username;

	UPROPERTY()
	FString gift_image;
	
	FInnerBasketPackage() : quantity(-1), price(-1.0f), gift_username_id(TEXT("")), gift_username(TEXT("")), gift_image(TEXT("")){}
};

USTRUCT()
struct FBasketPackage
{
	GENERATED_BODY()

	UPROPERTY()
	int32 id;

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString description;

	UPROPERTY()
	FInnerBasketPackage in_basket;
	
	FBasketPackage() : id(-1), name(TEXT("")), description(TEXT("")), in_basket(){}
};

USTRUCT()
struct FBasketCoupon
{
	GENERATED_BODY()

	UPROPERTY()
	FString coupon_code;

	FBasketCoupon() : coupon_code(TEXT("")){}

	FBasketCoupon(FString coupon_code) : coupon_code(coupon_code){}
};

USTRUCT()
struct FBasketGiftCard
{
	GENERATED_BODY()

	UPROPERTY()
	FString card_number;

	FBasketGiftCard() : card_number(TEXT("")){}
};

USTRUCT()
struct FBasketLinks
{
	GENERATED_BODY()

	UPROPERTY()
	FString payment;

	UPROPERTY()
	FString checkout;

	FBasketLinks() : payment(TEXT("")), checkout(TEXT(""))
	{
	}
};

USTRUCT()
struct FBasket
{
	GENERATED_BODY()
    
	UPROPERTY()
	FString id;

	UPROPERTY()
	FString ident;

	UPROPERTY()
	bool complete;

	UPROPERTY()
	FString email;

	UPROPERTY()
	FString username;

	UPROPERTY()
	TArray<FBasketCoupon> coupons;

	UPROPERTY()
	TArray<FBasketGiftCard> gift_cards;

	UPROPERTY()
	FString creator_code;

	UPROPERTY()
	FString cancel_url;

	UPROPERTY()
	FString complete_url;

	UPROPERTY()
	bool complete_auto_redirect;

	UPROPERTY()
	FString country;

	UPROPERTY()
	FString ip;

	UPROPERTY()
	int32 username_id;

	UPROPERTY()
	float base_price;

	UPROPERTY()
	float sales_tax;

	UPROPERTY()
	float total_price;

	UPROPERTY()
	FString currency;

	UPROPERTY()
	TArray<FBasketPackage> packages;

	UPROPERTY()
	TMap<FString, FString> custom;

	UPROPERTY()
	FBasketLinks links;

	FBasket() : id(TEXT("")), ident(TEXT("")), complete(false), email(TEXT("")), username(TEXT("")), coupons(), gift_cards(), creator_code(TEXT("")), cancel_url(TEXT("")), complete_url(TEXT("")), complete_auto_redirect(false), country(TEXT("")), ip(TEXT("")), username_id(-1), base_price(-1.0f), sales_tax(-1.0f), total_price(-1.0f), currency(TEXT("")), packages(), custom(), links()
	{
	}
};

USTRUCT()
struct FBasketAuthLink
{
	GENERATED_BODY()
	UPROPERTY()
	FString Name;
	
	UPROPERTY()
	FString Url;

	FBasketAuthLink() : Name(TEXT("")), Url(TEXT(""))
	{
	}
};

USTRUCT()
struct FWrappedBasket
{
	GENERATED_BODY()
	
	UPROPERTY()
	FBasket Data;
	FWrappedBasket() : Data()
	{
	}
};

USTRUCT()
struct FWrappedBasketLinks
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FBasketAuthLink> Data;

	FWrappedBasketLinks() : Data(){}
};