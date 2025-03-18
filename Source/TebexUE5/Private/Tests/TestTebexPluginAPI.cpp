#include "TebexPluginAPI.h"
#include "Misc/AutomationTest.h"
#include "Plugin/Models/ServerInformation.h"
#include "WaitForTest.h"

const FString TestSecretKey = TEXT("your-secret-key");
const FString TestOnlinePlayerName = TEXT("TebexDevUser");
const int TestPluginPackageId = 6051250;
const FString TestPluginPlayerId = "some-player-uuid";
const TArray<int> TestDeleteCommandIds = { 123456, 654321 };

class FTebexPluginApiTest : public FAutomationTestBase
{
public:
    FTebexPluginApiTest(const FString& InName, const bool bInComplexTask)
        : FAutomationTestBase(InName, bInComplexTask)
    {
        UTebexPluginAPI::SetSecretKey(TestSecretKey);
    }
};

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPIGetServerInfoTest, FTebexPluginApiTest, "Tebex.PluginAPI.GetServerInfo", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPIGetServerInfoTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FServerInformation>(this,
            [](TFunction<void(FServerInformation&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                UTebexPluginAPI::GetServerInformation([Callback](FServerInformation Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
            },
            [](FAutomationTestBase* Test, const FServerInformation& Data)
            {
                Test->TestTrue("Webstore callback triggered", true);
                Test->TestNotNull("Ensure Webstore object is not null", &Data);
                Test->TestTrue("Account domain is present", Data.account.domain.Len() > 0),
                Test->TestTrue("Account id is set", Data.account.id > 0),
                Test->TestTrue("Account ISO 4217 is set", Data.account.currency.iso_4217.Len() > 0),
                Test->TestTrue("Account currency symbol is set", Data.account.currency.symbol.Len() > 0),
                Test->TestTrue("Account game type is set", Data.account.game_type.Len() > 0),
                Test->TestTrue("Account name is set", Data.account.name.Len() > 0),
                Test->TestTrue("Server ID is set", Data.server.id > 0),
                Test->TestTrue("Server name is set", Data.server.name.Len() > 0);
            }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPIGetDuePlayersTest, FTebexPluginApiTest, "Tebex.PluginAPI.GetDuePlayers", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPIGetDuePlayersTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FDuePlayersResponse>(this,
            [](TFunction<void(FDuePlayersResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                UTebexPluginAPI::GetDuePlayers([Callback](FDuePlayersResponse Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
            },
            [](FAutomationTestBase* Test, const FDuePlayersResponse& Data)
            {
                Test->TestTrue("Due players callback triggered", true);
                Test->TestNotNull("Due players array is initialized", &Data.players);
                Test->TestNotNull("Due players meta is initialized", &Data.meta);
                Test->TestTrue("Next check value is assigned and valid",Data.meta.next_check > 0);
            }));
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPIGetOfflineCommandsTest, FTebexPluginApiTest, "Tebex.PluginAPI.GetOfflineCommands", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPIGetOfflineCommandsTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FOfflineCommandsResponse>(this,
            [](TFunction<void(FOfflineCommandsResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                UTebexPluginAPI::GetOfflineCommands([Callback](FOfflineCommandsResponse Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
            },
            [](FAutomationTestBase* Test, const FOfflineCommandsResponse& Data)
            {
                Test->TestNotNull("Offline commands array is initialized", &Data.commands);
                Test->TestNotNull("Offline commands meta is initialized", &Data.meta);
            }));
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPIGetOnlineComandsTest, FTebexPluginApiTest, "Tebex.PluginAPI.GetOnlineCommands", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPIGetOnlineComandsTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FOnlineCommandsResponse>(this,
            [](TFunction<void(FOnlineCommandsResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                UTebexPluginAPI::GetOnlineCommands(TestPluginPlayerId, [Callback](FOnlineCommandsResponse Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
            },
            [](FAutomationTestBase* Test, const FOnlineCommandsResponse& Data)
            {
                Test->TestNotNull("Online commands array is initialized", &Data.commands);
            }));
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPIDeleteCommandsTest, FTebexPluginApiTest, "Tebex.PluginAPI.DeleteCommands", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPIDeleteCommandsTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FEmptyBody>(this,
            [](TFunction<void(FEmptyBody&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                UTebexPluginAPI::DeleteCommands(TestDeleteCommandIds, [Callback](FEmptyBody Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
            },
            [](FAutomationTestBase* Test, const FEmptyBody& Data)
            {
                Test->TestTrue("Delete commands callback triggered", true);
            }));
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPICreateCheckoutUrlTest, FTebexPluginApiTest, "Tebex.PluginAPI.CreateCheckoutUrl", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPICreateCheckoutUrlTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FCheckoutResponse>(this,
            [](TFunction<void(FCheckoutResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                UTebexPluginAPI::CreateCheckoutUrl(TestPluginPackageId, TestOnlinePlayerName, [Callback](FCheckoutResponse Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
            },
            [](FAutomationTestBase* Test, const FCheckoutResponse& Data)
            {
                Test->TestTrue("Checkout expiry is assigned and valid", Data.expires.Len() > 0);
                Test->TestTrue("Checkout url is not empty", Data.url.Len() > 0);
            }));
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPIListActivePackagesTest, FTebexPluginApiTest, "Tebex.PluginAPI.ListActivePackages", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPIListActivePackagesTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FActivePackagesResponse>(this,
            [](TFunction<void(FActivePackagesResponse&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                UTebexPluginAPI::ListActivePackages(TestPluginPlayerId, [Callback](FActivePackagesResponse Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
            },
            [](FAutomationTestBase* Test, const FActivePackagesResponse& Data)
            {
                Test->TestNotNull("Packages are not null", &Data.packages);

                for (auto Package : Data.packages)
                {
                    Test->TestNotNull("Package record is not null", &Package.package);
                    Test->TestTrue("Package date is set", Package.date.Len() > 0);
                    Test->TestTrue("Package quantity is set and valid", Package.quantity > 0);
                    
                    auto InnerPackage = Package.package;
                    Test->TestTrue("Package ID is set and valid", InnerPackage.id > 0);
                    Test->TestTrue("Package name is set", InnerPackage.name.Len() > 0);
                }
            }));
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginAPISendEventsTest, FTebexPluginApiTest, "Tebex.PluginAPI.SendEvents", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
bool FTebexPluginAPISendEventsTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FEmptyBody>(this,
        [](TFunction<void(FEmptyBody&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
        {
            FJoinEvent TestEvent = FJoinEvent(TestPluginPlayerId, TestOnlinePlayerName, "127.0.0.1");
            FJoinEvents Events = FJoinEvents();
            Events.events.Add(TestEvent);
            
            UTebexPluginAPI::SendEvents(Events, [Callback](FEmptyBody Data)
            {
                Callback(Data);
            }, [ErrorCallback](FHttpResponsePtr ErrorResponse)
                {
                    ErrorCallback(ErrorResponse);
                });
        },
        [](FAutomationTestBase* Test, const FEmptyBody& Data)
        {
            Test->TestTrue("Send events callback triggered", true);
        }));
    return true;
}
