#pragma once

#include "CoreMinimal.h"
#include "Headless/EmptyBody.h"
#include "Plugin/Models/Commands.h"
#include "Plugin/Models/JoinEvent.h"
#include "Plugin/Models/ServerInformation.h"
#include "Util/SharedAliases.h"
#include "TebexPlugin.generated.h"

UCLASS()
class UTebexPlugin : public UObject
{
	GENERATED_BODY()

	public:
	static bool Enable();
	static void Shutdown();
	
	static void RecordJoinEvent(FString PlayerId, FString PlayerUsername);
	static bool DispatchCommand(const FString Command);
	static void DelayCommand(const FCommand Command);
	static void RecordCompletedCommand(const FCommand Command);
	
	static void RefreshServerInformation(TOkCallable<FServerInformation> Callback, FErrCallable Error);
	static void PerformCheck(TOkCallable<FEmptyBody> Callback, FErrCallable Error);
	static void DeleteCompletedCommands(TOkCallable<FEmptyBody> Callback, FErrCallable Error);
	static void SendJoinEvents(TOkCallable<FEmptyBody> Callback, FErrCallable Error);
	
	static void BeginRepeatingTask(int IntervalSeconds, TFunction<void()> Task);
	static void BeginDelayedTask(int DelaySeconds, TFunction<void()> Task);

	static FServerInformation GetServerInformation()
	{
		return ServerInformation;
	}
	
	static FJoinEvents GetJoinEvents()
	{
		return JoinEvents;
	}

	static TArray<FCommand> GetCompletedCommands()
	{
		return CompletedCommands;
	}
private:
	static FServerInformation ServerInformation;
	static FJoinEvents JoinEvents;
	static TArray<FCommand> CompletedCommands;
	static int LastCheck;
};
