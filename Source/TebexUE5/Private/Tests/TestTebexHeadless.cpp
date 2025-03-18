#include "TebexHeadlessAPI.h"
#include "WaitForTest.h"
#include "Headless/Models/Result.h"
#include "Misc/AutomationTest.h"

const FString TestPublicToken = TEXT("t66x-7cd928b1e9399909e6810edac6dc1fd1eefc57cb");

constexpr int32 TestPackageId = 6659238;
constexpr int32 TestCategoryId = 2678688;
const FString TestEmail = TEXT("tebex-integrations@overwolf.com");
const FString TestCreatorCode = TEXT("TebexDev");
const FString TestReturnUrl = TEXT("https://tebex.io/return");
const FString TestCouponCode = TEXT("COUPON123");

constexpr bool UseUsernames = false;
const FString TestUserName = TEXT("Tebex");

/**
 * FTebexBase test is the base class for all Headless API tests, which will set the public token before running.
 */
class FTebexHeadlessTest : public FAutomationTestBase
{
public:
    FTebexHeadlessTest(const FString& InName, const bool bInComplexTask)
        : FAutomationTestBase(InName, bInComplexTask)
    {
        UTebexHeadlessAPI::SetPublicToken(TestPublicToken);
    }
};

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexGetWebstoreTest, FTebexHeadlessTest, "Tebex.HeadlessAPI.GetWebstore", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexGetWebstoreTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FWebstore>(
        this,
        [](TFunction<void(FWebstore&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            UTebexHeadlessAPI::GetWebstore([Callback](FWebstore Data)
            {
                Callback(Data);
            }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
            {
                ErrorCallback(ErrorResponse);
            });
        },
        [](FAutomationTestBase* Test, const FWebstore& Data)
        {
            Test->TestTrue("Webstore callback triggered", true);
            Test->TestNotNull("Ensure Webstore object is not null", &Data);
            Test->TestTrue("Webstore ID should be greater than 0", Data.ID > 0);
            Test->TestFalse("Webstore name should not be empty", Data.Name.IsEmpty());
            Test->TestFalse("Webstore URL should not be empty", Data.Webstore_Url.IsEmpty());
            Test->TestNotNull("Description should not be nullptr", &Data.Description);
            Test->TestTrue("Webstore lang is set", !Data.Lang.IsEmpty());
            Test->TestTrue("Platform type is set", !Data.Platform_Type.IsEmpty());
            Test->TestTrue("Created at is set", !Data.Created_At.IsEmpty());
            Test->TestTrue("Platform type ID is provided", Data.Platform_Type_Id.IsNumeric());
        }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexGetAllPackagesTest, FTebexHeadlessTest, "Tebex.HeadlessAPI.GetAllPackages", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexGetAllPackagesTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FPackagesResponse>(
        this,
        [](TFunction<void(FPackagesResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            UTebexHeadlessAPI::GetAllPackages([Callback](FPackagesResponse Data)
            {
                Callback(Data);
            }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
            {
                ErrorCallback(ErrorResponse);
            });
        },
        [](FAutomationTestBase* Test, FPackagesResponse Data)
        {
            Test->TestTrue("GetAllPackages callback triggered", true);
            Test->TestTrue("Ensure Packages array is not empty", Data.data.Num() > 0);
            for (const FTebexPackage& P : Data.data)
            {
                Test->TestTrue("Package ID should be greater than 0", P.Id > 0);
                Test->TestFalse("Package name should not be empty", P.Name.IsEmpty());
                Test->TestFalse("Package description should not be empty", P.Description.IsEmpty());
                Test->TestFalse("Package type should not be empty", P.Type.IsEmpty());
                Test->TestTrue("Package category should be assigned", P.Category.Id > 0);
                Test->TestTrue("Package category has a description", !P.Category.Name.IsEmpty());
                Test->TestTrue("Package base price should be assigned", P.Base_Price >= 0.0f);
                Test->TestTrue("Package sales tax should be valid", P.Sales_Tax >= 0.0f);
                Test->TestTrue("Package quantity should be greater than 0", P.Total_Price >= 0.0f);
                Test->TestTrue("Package currency is set", !P.Currency.IsEmpty());
                Test->TestTrue("Package discount is valid", P.Discount >= 0.0);
                Test->TestTrue("Package creation date is provided", !P.Created_At.IsEmpty());
                Test->TestTrue("Package update date is provided", !P.Updated_At.IsEmpty());
                Test->TestTrue("Package order is valid", P.Order >= 0);
            }
        }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexGetPackageByIdTest, FTebexHeadlessTest, "Tebex.HeadlessAPI.GetPackageById", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexGetPackageByIdTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FWrappedPackage>(
        this,
        [](TFunction<void(FWrappedPackage&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            UTebexHeadlessAPI::GetPackageById(TestPackageId, [Callback](FWrappedPackage Data)
            {
                Callback(Data);
            }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
            {
                ErrorCallback(ErrorResponse);
            });
        },
        [](FAutomationTestBase* Test, const FWrappedPackage& Data)
        {
            const FTebexPackage P = Data.data;
            Test->TestTrue("GetPackageById callback triggered", true);
            Test->TestTrue("Package ID should be greater than 0", P.Id > 0);
            Test->TestFalse("Package name should not be empty", P.Name.IsEmpty());
            Test->TestFalse("Package description should not be empty", P.Description.IsEmpty());
            Test->TestFalse("Package type should not be empty", P.Type.IsEmpty());
            Test->TestTrue("Package category should be assigned", P.Category.Id > 0);
            Test->TestTrue("Package category has a description", !P.Category.Name.IsEmpty());
            Test->TestTrue("Package base price should be assigned", P.Base_Price >= 0.0f);
            Test->TestTrue("Package sales tax should be valid", P.Sales_Tax >= 0.0f);
            Test->TestTrue("Package quantity should be greater than 0", P.Total_Price >= 0.0f);
            Test->TestTrue("Package currency is set", !P.Currency.IsEmpty());
            Test->TestTrue("Package discount is valid", P.Discount >= 0.0);
            Test->TestTrue("Package creation date is provided", !P.Created_At.IsEmpty());
            Test->TestTrue("Package update date is provided", !P.Updated_At.IsEmpty());
            Test->TestTrue("Package order is valid", P.Order >= 0);     
        }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexGetAllCategoriesTest, FTebexHeadlessTest, "Tebex.HeadlessAPI.GetAllCategories", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexGetAllCategoriesTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FCategoriesResponse>(
        this,
        [](TFunction<void(FCategoriesResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            UTebexHeadlessAPI::GetAllCategories([Callback](FCategoriesResponse Data)
            {
                Callback(Data);
            },[ErrorCallback](FHttpResponsePtr ErrorResponse)
            {
                ErrorCallback(ErrorResponse);
            });
        },
        [](FAutomationTestBase* Test, FCategoriesResponse Data)
        {
            Test->TestTrue("GetAllCategories callback triggered", true);
            

            //FIXME
            //Test->TestTrue("Categories array should not be empty", Data.data > 0);
            // for (FCategory C : Data)
            // {
            //     Test->TestTrue("Category ID should be greater than 0", C.Id > -1);
            //     Test->TestTrue("Category name should not be empty", C.Name.Len() > 0);
            //     Test->TestTrue("Category order should be valid", C.Order > -1);
            // }
        }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexGetAllCategoriesIncludingPackagesTest, FTebexHeadlessTest, "Tebex.HeadlessAPI.GetAllCategoriesIncludingPackages", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexGetAllCategoriesIncludingPackagesTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FCategoriesResponse>(
        this,
        [](TFunction<void(FCategoriesResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            UTebexHeadlessAPI::GetAllCategoriesIncludingPackages([Callback](FCategoriesResponse Data)
            {
                Callback(Data);
            }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
            {
                ErrorCallback(ErrorResponse);
            });
        },
        [](FAutomationTestBase* Test, FCategoriesResponse Data)
        {
            Test->TestTrue("GetAllCategoriesIncludingPackages callback triggered", true);
            //FIXME
            // Test->TestTrue("Categories array should not be empty", Data.() > 0);
            // for (FCategory C : Data)
            // {
            //     Test->TestTrue("Category ID should be greater than 0", C.Id > -1);
            //     Test->TestTrue("Category name should not be empty", C.Name.Len() > 0);
            //     Test->TestTrue("Category order should be valid", C.Order > -1);
            // }
        }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexGetCategoryTest, FTebexHeadlessTest, "Tebex.HeadlessAPI.GetCategory", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexGetCategoryTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FCategory>(
        this,
        [](TFunction<void(FCategory&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            UTebexHeadlessAPI::GetCategory(TestCategoryId, [Callback](FCategory Data)
            {
                Callback(Data);
            },[ErrorCallback](FHttpResponsePtr ErrorResponse)
            {
                ErrorCallback(ErrorResponse);
            });
        },
        [](FAutomationTestBase* Test, FCategory Data)
        {
            Test->TestTrue("GetCategory callback triggered", true);
            Test->TestTrue("Category ID should be greater than 0", Data.id > -1);
            Test->TestTrue("Category name should not be empty", Data.name.Len() > 0);
            Test->TestTrue("Category order should be valid", Data.order > -1);
        }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexCreateAndUseBasketTest, FTebexHeadlessTest, "Tebex.HeadlessAPI.CreateAndUseBasket", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

bool FTebexCreateAndUseBasketTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FBasket>(
        this,
        [](TFunction<void(FBasket&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            FCreateBasketPayload CreationData;
            CreationData.complete_url = "https://tebex.io/complete";
            CreationData.cancel_url = "https://tebex.io/return";
            CreationData.email = TestEmail;

            if (UseUsernames)
            {
                CreationData.username = TestUserName;
            }

            UTebexHeadlessAPI::CreateBasket(CreationData, [Callback](FBasket Data)
            {
                Callback(Data);
            },
            [ErrorCallback](FHttpResponsePtr ErrorResponse)
            {
                ErrorCallback(ErrorResponse);
            });
        },
        [this](FAutomationTestBase* BasketTest, const FBasket& ReceivedBasket)
        {
            BasketTest->TestTrue("Basket creation succeeded", !ReceivedBasket.ident.IsEmpty());

            // Add a base package
            ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FBasket>(
                this,
                [ReceivedBasket](TFunction<void(FBasket&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                {
                    FTebexPackage TestPackage;
                    TestPackage.Id = TestPackageId;

                    UTebexHeadlessAPI::AddPackageToBasket(ReceivedBasket, TestPackage, 2, [Callback](FBasket BasketData)
                    {
                        Callback(BasketData);
                    },
                    [ErrorCallback](FHttpResponsePtr ErrorResponse)
                    {
                        ErrorCallback(ErrorResponse);
                    });
                },
                [this](FAutomationTestBase* AddPackageTest, const FBasket& UpdatedBasket)
                {
                    AddPackageTest->TestTrue("Package added to basket", UpdatedBasket.packages.Num() > 0);
                    AddPackageTest->TestTrue("Package quantity is correct", UpdatedBasket.packages[0].in_basket.quantity == 2);

                    // Remove that package
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FBasket>(
                        this,
                        [UpdatedBasket](TFunction<void(FBasket&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                        {
                            FTebexPackage TestPackage;
                            TestPackage.Id = TestPackageId;

                            UTebexHeadlessAPI::RemovePackageFromBasket(UpdatedBasket, TestPackage, [Callback](FBasket BasketData)
                            {
                                Callback(BasketData);
                            },
                            [ErrorCallback](FHttpResponsePtr ErrorResponse)
                            {
                                ErrorCallback(ErrorResponse);
                            });
                        },
                        [this](FAutomationTestBase* RemovePackageTest, const FBasket& RemovedPackageBasket)
                        {
                            RemovePackageTest->TestTrue("Package removed from basket", RemovedPackageBasket.packages.Num() == 0);

                            // Apply creator code
                            ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FResult>(
                                this,
                                [RemovedPackageBasket](TFunction<void(FResult&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                                {
                                    UTebexHeadlessAPI::ApplyCreatorCode(RemovedPackageBasket, TestCreatorCode, [Callback](FResult CreatorCodeResponse)
                                    {
                                        Callback(CreatorCodeResponse);
                                    },
                                    [ErrorCallback](FHttpResponsePtr ErrorResponse)
                                    {
                                        ErrorCallback(ErrorResponse);
                                    });
                                },
                                [this, RemovedPackageBasket](FAutomationTestBase* CreatorCodeTest, const FResult& CreatorCodeResponse)
                                {
                                    CreatorCodeTest->TestTrue("Creator code added to basket", CreatorCodeResponse.success);

                                    // Remove creator code
                                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FResult>(
                                        this,
                                        [RemovedPackageBasket](TFunction<void(FResult&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                                        {
                                            UTebexHeadlessAPI::RemoveCreatorCode(RemovedPackageBasket, [Callback](FResult CreatorCodeRemoveResponse)
                                            {
                                                Callback(CreatorCodeRemoveResponse);
                                            },
                                            [ErrorCallback](FHttpResponsePtr ErrorResponse)
                                            {
                                                ErrorCallback(ErrorResponse);
                                            });
                                        },
                                        [this, RemovedPackageBasket](FAutomationTestBase* RemoveCreatorCodeTest, const FResult& CreatorCodeRemoveResponse)
                                        {
                                            RemoveCreatorCodeTest->TestTrue("Creator code removed from basket", CreatorCodeRemoveResponse.success);

                                            // Add another package
                                            ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FBasket>(
                                                this,
                                                [RemovedPackageBasket](TFunction<void(FBasket&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                                                {
                                                    FTebexPackage TestPackage;
                                                    TestPackage.Id = TestPackageId;

                                                    UTebexHeadlessAPI::AddPackageToBasket(RemovedPackageBasket, TestPackage, 1, [Callback](FBasket BasketData)
                                                    {
                                                        Callback(BasketData);
                                                    },
                                                    [ErrorCallback](FHttpResponsePtr ErrorResponse)
                                                    {
                                                        ErrorCallback(ErrorResponse);
                                                    });
                                                },
                                                [this](FAutomationTestBase* AddAnotherPackageTest, const FBasket& SecondUpdatedBasket)
                                                {
                                                    AddAnotherPackageTest->TestTrue("Package re-added to basket", SecondUpdatedBasket.packages.Num() > 0);

                                                    // Use a coupon
                                                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FResult>(
                                                        this,
                                                        [SecondUpdatedBasket](TFunction<void(FResult&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                                                        {
                                                            UTebexHeadlessAPI::ApplyCoupon(SecondUpdatedBasket, TestCouponCode, [Callback](FResult Response)
                                                            {
                                                                Callback(Response);
                                                            },
                                                            [ErrorCallback](FHttpResponsePtr ErrorResponse)
                                                            {
                                                                ErrorCallback(ErrorResponse);
                                                            });
                                                        },
                                                        [this, SecondUpdatedBasket](FAutomationTestBase* ApplyCouponTest, const FResult& Response)
                                                        {
                                                            ApplyCouponTest->TestTrue("Coupon is applied", Response.success);

                                                            // Remove the coupon
                                                            ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FResult>(
                                                                this,
                                                                [SecondUpdatedBasket](TFunction<void(FResult&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                                                                {
                                                                    UTebexHeadlessAPI::RemoveCoupon(SecondUpdatedBasket, TestCouponCode, [Callback](FResult Response)
                                                                    {
                                                                        Callback(Response);
                                                                    },
                                                                    [ErrorCallback](FHttpResponsePtr ErrorResponse)
                                                                    {
                                                                        ErrorCallback(ErrorResponse);
                                                                    });
                                                                },
                                                                [this, SecondUpdatedBasket](FAutomationTestBase* RemoveCouponTest, const FResult& Response)
                                                                {
                                                                    RemoveCouponTest->TestTrue("Coupon is removed", Response.success);

                                                                    // Get basket
                                                                     ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FBasket>(
                                                                         this,
                                                                         [SecondUpdatedBasket](TFunction<void(FBasket&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                                                                         {
                                                                             UTebexHeadlessAPI::GetBasket(SecondUpdatedBasket.ident, [Callback](FBasket Basket)
                                                                             {
                                                                                 Callback(Basket);
                                                                             }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                                                                             {
                                                                                 ErrorCallback(ErrorResponse);
                                                                             });
                                                                         },
                                                                         [this](FAutomationTestBase* GetBasketTest, const FBasket& Basket)
                                                                         {
                                                                             GetBasketTest->TestFalse("Id should be present", Basket.id.IsEmpty());
                                                                             GetBasketTest->TestFalse("Ident should be present", Basket.ident.IsEmpty());
                                                                             GetBasketTest->TestEqual("Complete should be false", Basket.complete, false);
                                                                             GetBasketTest->TestFalse("Email should be present", Basket.email.IsEmpty());
                                                                             GetBasketTest->TestNotNull("Username should not be null", &Basket.username);
                                                                             GetBasketTest->TestTrue("Coupons array should be empty", Basket.coupons.Num() == 0);
                                                                             GetBasketTest->TestTrue("Gift cards array should be empty", Basket.gift_cards.Num() == 0);
                                                                             GetBasketTest->TestEqual("Creator code should be empty", Basket.creator_code, TEXT(""));
                                                                             GetBasketTest->TestTrue("Cancel url should be present", Basket.cancel_url.Len() > 0);
                                                                             GetBasketTest->TestTrue("Complete url should be present", Basket.complete_url.Len() > 0);
                                                                             GetBasketTest->TestEqual("CompleteAutoRedirect should be false", Basket.complete_auto_redirect, false);
                                                                             GetBasketTest->TestFalse("Country should be present", Basket.country.IsEmpty());
                                                                             GetBasketTest->TestFalse("Ip should be present", Basket.ip.IsEmpty());
                                                                             GetBasketTest->TestNotEqual("Base_Price should not be default -1.0f", Basket.base_price, -1.0f);
                                                                             GetBasketTest->TestNotEqual("Sales_Tax should not be default -1.0f", Basket.sales_tax, -1.0f);
                                                                             GetBasketTest->TestNotEqual("Total_Price should not be default -1.0f", Basket.total_price, -1.0f);
                                                                             GetBasketTest->TestTrue("Currency should be present", Basket.currency.Len() > 0);
                                                                             GetBasketTest->TestTrue("Custom map should be initialized empty", Basket.custom.Num() == 0);
                                                                             
                                                                             // Get auth links
                                                                             // ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FBasketLinks>(
                                                                             //     this,
                                                                             //     [Basket](TFunction<void(FBasketLinks&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                                                                             //     {
                                                                             //         UTebexHeadlessAPI::GetBasketAuthLinks(Basket, TestReturnUrl, [Callback](FBasketLinks BasketLinks)
                                                                             //         {
                                                                             //             Callback(BasketLinks);
                                                                             //         }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                                                                             //         {
                                                                             //             ErrorCallback(ErrorResponse);
                                                                             //         });
                                                                             //     },
                                                                             //     [](FAutomationTestBase* BasketAuthLinksTest, const FBasketLinks& BasketLinks)
                                                                             //     {
                                                                             //         BasketAuthLinksTest->TestFalse("Checkout URL is present", BasketLinks.checkout.IsEmpty());
                                                                             //         BasketAuthLinksTest->TestFalse("Payment URL is present", BasketLinks.payment.IsEmpty());
                                                                             //     }
                                                                             // ));
                                                                         }
                                                                     ));
                                                                }
                                                            ));
                                                        }
                                                    ));
                                                }
                                            ));
                                        }
                                    ));
                                }
                            ));
                        }
                    ));
                }
            ));
        }
    ));
    return true;
}