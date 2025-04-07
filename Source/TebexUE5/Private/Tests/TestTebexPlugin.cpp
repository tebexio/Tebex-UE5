#include "TebexPlugin.h"
#include "TebexPluginAPI.h"
#include "Misc/AutomationTest.h"
#include "Plugin/Models/ServerInformation.h"
#include "WaitForTest.h"

const FString TestPluginPublicToken = TEXT("<KEY>");
const FString TestPluginSecretKey = TEXT("<KEY>");

class FTebexPluginTest : public FAutomationTestBase
{
public:
    UTebexPlugin* Plugin;
    
    FTebexPluginTest(const FString& InName, const bool bInComplexTask)
        : FAutomationTestBase(InName, bInComplexTask)
    {
        UTebexPluginAPI::SetSecretKey(TestPluginSecretKey);
        Plugin = NewObject<UTebexPlugin>();
    }
};

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginEnableTest, FTebexPluginTest, "Tebex.Plugin.Enable", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
bool FTebexPluginEnableTest::RunTest(const FString& Parameters)
{
    TestTrue("Plugin is enabled successfully", Plugin->Enable());
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexRecordCompletedCommandTest, FTebexPluginTest, "Tebex.Plugin.RecordCompletedCommand", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
bool FTebexRecordCompletedCommandTest::RunTest(const FString& Parameters)
{
    TestTrue("Plugin completed commands is empty", Plugin->GetCompletedCommands().Num() == 0);
    FCommand example;
    example.id = 1234567;
    Plugin->RecordCompletedCommand(example);
    TestTrue("Plugin completed commands is not empty", Plugin->GetCompletedCommands().Num() == 1);

    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FEmptyBody>(this,
            [this](TFunction<void(FEmptyBody&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
            {
                Plugin->DeleteCompletedCommands([Callback](FEmptyBody Data)
                {
                    Callback(Data);
                }, [ErrorCallback](FHttpResponsePtr Error)
                {
                    ErrorCallback(Error);
                });
            },
            [this](FAutomationTestBase* Test, const FEmptyBody& Data)
            {
                Test->TestTrue("Plugin completed commands is empty after deletion", Plugin->GetCompletedCommands().Num() == 0);
            }));
    
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginRecordJoinEventTest, FTebexPluginTest, "Tebex.Plugin.RecordJoinEvent", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
bool FTebexPluginRecordJoinEventTest::RunTest(const FString& Parameters)
{
    TestTrue("Plugin join events is empty", Plugin->GetJoinEvents().events.Num() == 0);
    Plugin->RecordJoinEvent("1234567", "TebexDev");
    TestTrue("Plugin records a join event", Plugin->GetJoinEvents().events.Num() == 1);
    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginRefreshServerInformationTest, FTebexPluginTest, "Tebex.Plugin.RefreshServerInformation", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
bool FTebexPluginRefreshServerInformationTest::RunTest(const FString& Parameters)
{
    TestTrue("Plugin started with no server information", Plugin->GetServerInformation().server.name.Len() == 0);
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FServerInformation>(this,
                [](TFunction<void(FServerInformation&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                {
                    UTebexPlugin::RefreshServerInformation([Callback](FServerInformation Data)
                    {
                        Callback(Data);
                    }, [ErrorCallback](FHttpResponsePtr Error)
                    {
                        ErrorCallback(Error);
                    });
                },
                [this](FAutomationTestBase* Test, const FServerInformation& Data)
                {
                    Test->TestTrue("Plugin refreshed server information", Plugin->GetServerInformation().server.name.Len() > 0);
                }));

    return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FTebexPluginPerformCheckTest, FTebexPluginTest, "Tebex.Plugin.PerformCheck", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
bool FTebexPluginPerformCheckTest::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitForTest<FEmptyBody>(this,
                [](TFunction<void(FEmptyBody&)> Callback, TFunction<void(FHttpResponsePtr&)> ErrorCallback)
                {
                    UTebexPlugin::PerformCheck([Callback](FEmptyBody Data)
                    {
                        Callback(Data);
                    }, [ErrorCallback](FHttpResponsePtr Error)
                    {
                        ErrorCallback(Error);
                    });
                },
                [this](FAutomationTestBase* Test, const FEmptyBody& Data)
                {
                    Test->TestTrue("Plugin performed check callback", true);
                }));

    return true;
}