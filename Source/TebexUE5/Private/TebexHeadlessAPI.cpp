#include "TebexHeadlessAPI.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "TebexUE5.h"
#include "Headless/EmptyBody.h"
#include "Headless/Models/Promotions.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Tests/Debug.h"

DEFINE_LOG_CATEGORY(LogTebex)

FString UTebexHeadlessAPI::PublicToken = TEXT("");
const FString UTebexHeadlessAPI::BaseUrl = TEXT("https://headless.tebex.io/api/");
bool UTebexHeadlessAPI::TokenIsSet = false;

void UTebexHeadlessAPI::GetWebstore( TOkCallable<FWebstore> Callback, FErrCallable Error )
{
	const FString Url = BaseUrl + "accounts/" + PublicToken;
	MakeRequest<FWebstoreResponse, FEmptyBody>(Url, "GET", [Callback](const FWebstoreResponse& Response) {
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::GetAllPackages(TOkCallable<FPackagesResponse> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/packages";
	MakeRequest<FPackagesResponse, FEmptyBody>(Url, "GET", [Callback](const FPackagesResponse& Response) {
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::GetPackageById(const int PackageId, TOkCallable<FWrappedPackage> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/packages/" + FString::FromInt(PackageId);
	MakeRequest<FWrappedPackage, FEmptyBody>(Url, "GET", [Callback](const FWrappedPackage& Response) {
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::GetPackageById(const int PackageId, const FString& BasketIdent, TOkCallable<FWrappedPackage> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/packages/" + FString::FromInt(PackageId) + "?basketIdent=" + BasketIdent;
	MakeRequest<FWrappedPackage, FEmptyBody>(Url, "GET", [Callback](const FWrappedPackage& Response) {
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::GetPackageById(const int PackageId, const FString& BasketIdent, const FString& IpAddress, TOkCallable<FWrappedPackage> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/packages/" + FString::FromInt(PackageId) + "?basketIdent=" + BasketIdent + "&ipAddress=" + IpAddress;
	MakeRequest<FWrappedPackage, FEmptyBody>(Url, "GET", [Callback](const FWrappedPackage& Response) {
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::GetBasket(const FString& Ident, TOkCallable<FBasket> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Ident;
	MakeRequest<FWrappedBasket, FEmptyBody>(Url, "GET", [Callback](const FWrappedBasket& Response) {
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::CreateBasket(const FCreateBasketPayload& Payload, TOkCallable<FBasket> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets";
	MakeRequest<FWrappedBasket, FCreateBasketPayload>(Url, "POST", Payload, [Callback](const FWrappedBasket& Response) {
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::AddPackageToBasket(const FBasket& Basket, const FTebexPackage& Package, const int32 Quantity, TOkCallable<FBasket> Callback, FErrCallable Error)
{
	FAddPackagePayload AddData;
	AddData.package_id = Package.Id;
	AddData.quantity = Quantity;
	
	const FString Url = BaseUrl + "baskets/" + Basket.ident + "/packages";
	MakeRequest<FWrappedBasket, FAddPackagePayload>(Url, "POST", AddData, [Callback](const FWrappedBasket& Response) {
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::UpdatePackageQuantity(const FBasket& Basket, const FTebexPackage& Package, const int NewQuantity, TOkCallable<FBasket> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "baskets/" + Basket.ident + "/packages/" + FString::FromInt(Package.Id);
	FPackageQuantity Quantity = FPackageQuantity(NewQuantity);
	MakeRequest<FWrappedBasket, FPackageQuantity>(Url, "PUT", Quantity, [Callback](const FWrappedBasket& Response) {
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::RemovePackageFromBasket(const FBasket& Basket, const FTebexPackage& Package, TOkCallable<FBasket> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "baskets/" + Basket.ident + "/packages/remove";
	FPackageRemovePayload RemoveData;
	RemoveData.package_id = FString::FromInt(Package.Id);
	MakeRequest<FWrappedBasket, FPackageRemovePayload>(Url, "POST", RemoveData, [Callback](const FWrappedBasket& Response) {
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::GetBasketAuthLinks(const FBasket& Basket, const FString& ReturnUrl, TOkCallable<FBasketLinks> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Basket.ident + "/auth?returnUrl=" + ReturnUrl;
	MakeRequest<FBasketLinks, FEmptyBody>(Url, "GET", [Callback](const FBasketLinks& Response) {
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::GetCategory(const int CategoryId, TOkCallable<FCategory> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/categories/" + FString::FromInt(CategoryId);
	MakeRequest<FWrappedCategory, FEmptyBody>(Url, "GET", [Callback](const FWrappedCategory& Response) {
		Callback(Response.data);
	}, Error);
}

void UTebexHeadlessAPI::GetCategoryIncludingPackages(const int CategoryId, TOkCallable<FCategory> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/categories/" + FString::FromInt(CategoryId) + "?includePackages=1";
	MakeRequest<FWrappedCategory, FEmptyBody>(Url, "GET", [Callback](const FWrappedCategory& Response) {
		Callback(Response.data);
	}, Error);
}

void UTebexHeadlessAPI::GetAllCategories(TOkCallable<FCategoriesResponse> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/categories";
	MakeRequest<FCategoriesResponse, FEmptyBody>(Url, "GET", [Callback](const FCategoriesResponse& Response)
	{
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::GetAllCategoriesIncludingPackages(TOkCallable<FCategoriesResponse> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/categories?includePackages=1";
	MakeRequest<FCategoriesResponse, FEmptyBody>(Url, "GET", [Callback](const FCategoriesResponse& Response)
	{
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::ApplyCreatorCode(const FBasket& Basket, const FString& CreatorCode, TOkCallable<FResult> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Basket.ident + "/creator-codes";
	FCreatorCode CreatorCodeData = FCreatorCode(CreatorCode);
	MakeRequest<FResult, FCreatorCode>(Url, "POST", CreatorCodeData, [Callback](const FResult& Response)
	{
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::RemoveCreatorCode(const FBasket& Basket, TOkCallable<FResult> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Basket.ident + "/creator-codes/remove";
	MakeRequest<FResult, FEmptyBody>(Url, "POST", FTebexUE5::EmptyRequest, [Callback](const FResult& Response)
	{
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::ApplyGiftCard(const FBasket& Basket, const FGiftCardNumber& GiftCardNumber, TOkCallable<FBasket> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Basket.ident + "/gift-cards";
	MakeRequest<FWrappedBasket, FGiftCardNumber>(Url, "POST", GiftCardNumber, [Callback](const FWrappedBasket& Response)
	{
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::RemoveGiftCard(const FBasket& Basket, const FGiftCardNumber& GiftCardNumber, TOkCallable<FBasket> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Basket.ident + "/gift-cards/remove";
	MakeRequest<FWrappedBasket, FGiftCardNumber>(Url, "POST", GiftCardNumber, [Callback](const FWrappedBasket& Response)
	{
		Callback(Response.Data);
	}, Error);
}

void UTebexHeadlessAPI::ApplyCoupon(const FBasket& Basket, const FString& CouponCode, TOkCallable<FResult> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Basket.ident + "/coupons";
	FBasketCoupon Coupon = FBasketCoupon(CouponCode);
	MakeRequest<FResult, FBasketCoupon>(Url, "POST", Coupon, [Callback](const FResult& Response)
	{
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::RemoveCoupon(const FBasket& Basket, const FString& CouponCode, TOkCallable<FResult> Callback, FErrCallable Error)
{
	const FString Url = BaseUrl + "accounts/" + PublicToken + "/baskets/" + Basket.ident + "/coupons/remove";
	const FBasketCoupon Coupon = FBasketCoupon(CouponCode);
	MakeRequest<FResult, FBasketCoupon>(Url, "POST", Coupon, [Callback](const FResult& Response)
	{
		Callback(Response);
	}, Error);
}

void UTebexHeadlessAPI::SetPublicToken(FString Token)
{
	if (Token.IsEmpty())
	{
		FTebexUE5::Log("Public token cannot be empty.");
		return;
	}
	
	PublicToken = Token;
	TokenIsSet = true;
}

template <typename ReturnType, typename RequestBodyType>
void UTebexHeadlessAPI::MakeRequest(const FString& Url, const FString& Verb, TFunction<void(const ReturnType&)> Callback, FErrCallable ErrorCallback)
{
	MakeRequest<ReturnType, RequestBodyType>(Url, Verb, FTebexUE5::EmptyRequest, Callback, ErrorCallback);
}

template <typename ReturnType, typename RequestBodyType>
void UTebexHeadlessAPI::MakeRequest(const FString& Url, const FString& Verb, TFunction<void(const ReturnType&)> Callback)
{
	MakeRequest<ReturnType, RequestBodyType>(Url, Verb, FTebexUE5::EmptyRequest, Callback);
}

template <typename ReturnType, typename RequestBodyType>
void UTebexHeadlessAPI::MakeRequest(const FString& Url, const FString& Verb, RequestBodyType SerializableRequestBody, TFunction<void(const ReturnType&)> OnSuccess, FErrCallable OnError)
{
	if (!TokenIsSet) // Require a store's public token to be set for all requests.
	{
		FTebexUE5::Log("Public token is not set.");
		return;
	}

	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) // Ensure we have HTTP available (we're not in the middle of shutdown / http is not included)
 	{
		FTebexUE5::Log("HTTP Module not initialized!");
		return;
	}

	// Build the request
	const TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader("Content-Type", "application/json");

	// If data is provided in request body, serialize the incoming request body as JSON and set the request content
	FString RequestBodyJson = TEXT("");
	if (!std::is_same_v<RequestBodyType, FEmptyBody>)
	{
		if (FJsonObjectConverter::UStructToJsonObjectString(RequestBodyType::StaticStruct(), &SerializableRequestBody, RequestBodyJson))
		{
			Request->SetContentAsString(RequestBodyJson);
		}
		else
		{
			FTebexUE5::Log("Failed to serialize request body: " + RequestBodyJson);
			return;
		}
	}

	// Log the request sent
	FTebexUE5::Log("-> " + Verb + " " + Url);
	if (RequestBodyJson.Len() > 0)
	{
		FTebexUE5::Log("  with body: " + RequestBodyJson);	
	}
	
	// Bind a function to consume the response. All responses are expected to be JSON, so we will parse the response
	// into the expected StructType template provided with the request call.
	Request->OnProcessRequestComplete().BindLambda(
		[Verb, OnSuccess, OnError](const FHttpRequestPtr& CompletedRequest, const FHttpResponsePtr& Response, const bool bWasSuccessful)
		{
			const FString ResponseBody = Response->GetContentAsString();
			const int ResponseCode = Response->GetResponseCode();

			FTebexUE5::Log("<- " + FString::FromInt(ResponseCode) + " " + Verb + " " + CompletedRequest->GetURL() + " with body: " + ResponseBody);
			
			if (!bWasSuccessful)
			{
				if (OnError)
				{
					OnError(Response);	
				}
				return;
			}
			
			if (ResponseCode >= 200 && ResponseCode <= 204)
			{
				// On success, parse the JSON response into the expected return type
				ReturnType ParsedResponse;
				if (FJsonObjectConverter::JsonObjectStringToUStruct<ReturnType>(
					   ResponseBody,
					   &ParsedResponse,
					   0, // No flags
					   0)) // No CheckFlags
				{
					if (OnSuccess)
					{
						PrintUStructProperties(ParsedResponse);
						OnSuccess(ParsedResponse); // Pass parsed struct to the callback func
					}
				}
				else // Fails to parse response into JSON
				{
					FTebexUE5::Log("Failed to parse response body as JSON.");
					if (OnError)
					{
						OnError(Response);	
					}					
				}
			}
			else // Non-success response code
			{
				if (OnError)
				{
					OnError(Response);
				}
			}
		});

	// Executes the HTTP request. This is async, which returns when the request is successfully started.
	if (!Request->ProcessRequest())
	{
		FTebexUE5::Log("Request failed to process!");
	}
}