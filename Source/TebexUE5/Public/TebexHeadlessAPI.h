#pragma once

#include "CoreMinimal.h"
#include "Util/SharedAliases.h"
#include "Headless/Models/Webstore.h"
#include "Headless/Models/Basket.h"
#include "Headless/Models/Package.h"
#include "Headless/Models/Category.h"
#include "Headless/Models/Promotions.h"
#include "Headless/Models/Result.h"
#include "TebexHeadlessAPI.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTebex, Log, All);

/**
 * The "Headless API" is for implementing your own frontend with the Tebex packages defined in your store. Retrieve package
 * information, build baskets, and direct users to checkout using this API and a public token for your store.
 */
UCLASS()
class UTebexHeadlessAPI : public UObject
{
	GENERATED_BODY()

public:
	static const FString BaseUrl;
	
	static void GetWebstore(TOkCallable<FWebstore> Callback, FErrCallable Error);
	static void GetAllPackages(TOkCallable<FPackagesResponse> Callback, FErrCallable Error);
	static void GetPackageById(int PackageId, TOkCallable<FWrappedPackage> Callback, FErrCallable Error);
	static void GetPackageById(int PackageId, const FString& BasketIdent, TOkCallable<FWrappedPackage> Callback, FErrCallable Error);
	static void GetPackageById(int PackageId, const FString& BasketIdent, const FString& IpAddress, TOkCallable<FWrappedPackage> Callback, FErrCallable Error);
	static void GetAllCategories(TOkCallable<FCategoriesResponse> Callback, FErrCallable Error);
	static void GetAllCategoriesIncludingPackages(TOkCallable<FCategoriesResponse> Callback, FErrCallable Error);
	static void GetCategory(int CategoryId, TOkCallable<FCategory> Callback, FErrCallable Error);
	static void GetCategoryIncludingPackages(int CategoryId, TOkCallable<FCategory> Callback, FErrCallable Error);
	static void GetBasket(const FString& Ident, TOkCallable<FBasket> Callback, FErrCallable Error);
	static void CreateBasket(const FCreateBasketPayload& Payload, TOkCallable<FBasket> Callback, FErrCallable Error);
	static void AddPackageToBasket(const FBasket& Basket, const FTebexPackage& Package, int32 Quantity, TOkCallable<FBasket> Callback, FErrCallable Error);
	static void UpdatePackageQuantity(const FBasket& Basket, const FTebexPackage& Package, int NewQuantity, TOkCallable<FBasket> Callback, FErrCallable Error);
	static void GetBasketAuthLinks(const FBasket& Basket, const FString& ReturnUrl, TOkCallable<FBasketLinks> Callback, FErrCallable Error);
	static void RemovePackageFromBasket(const FBasket& Basket, const FTebexPackage& Package, TOkCallable<FBasket> Callback, FErrCallable Error);
	static void ApplyCreatorCode(const FBasket& Basket, const FString& CreatorCode, TOkCallable<FResult> Callback, FErrCallable Error);
	static void RemoveCreatorCode(const FBasket& Basket, TOkCallable<FResult> Callback, FErrCallable Error);
	static void ApplyGiftCard(const FBasket& Basket, const FGiftCardNumber& GiftCardNumber, TOkCallable<FBasket> Callback, FErrCallable Error);
	static void RemoveGiftCard(const FBasket& Basket, const FGiftCardNumber& GiftCardNumber, TOkCallable<FBasket> Callback, FErrCallable Error);
	static void ApplyCoupon(const FBasket& Basket, const FString& CouponCode, TOkCallable<FResult> Callback, FErrCallable Error);
	static void RemoveCoupon(const FBasket& Basket, const FString& CouponCode, TOkCallable<FResult> Callback, FErrCallable Error);
	static void GetTieredCategoriesForUser(const FString& UsernameId, TOkCallable<FCategoriesResponse> Callback, FErrCallable Error);
	static void GetTieredCategories(TOkCallable<FCategoriesResponse> Callback, FErrCallable Error);
	static void UpdateTier(const int TierId, const int NewTierPackageId, TOkCallable<FUpdateTierResponse> Callback, FErrCallable Error);
	static void SetPublicToken(FString Token);
private:
	static FString PublicToken;
	static bool TokenIsSet;

	template <typename ReturnType, typename RequestBodyType>
	static void MakeRequest(const FString& Url, const FString& Verb, TFunction<void(const ReturnType&)> Callback);

	template <typename ReturnType, typename RequestBodyType>
	static void MakeRequest(const FString& Url, const FString& Verb, TFunction<void(const ReturnType&)> Callback, FErrCallable ErrorCallback);
	
	template <typename ReturnType, typename RequestBodyType>
	static void MakeRequest(const FString& Url, const FString& Verb, RequestBodyType SerializableRequestBody, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable Error);
};