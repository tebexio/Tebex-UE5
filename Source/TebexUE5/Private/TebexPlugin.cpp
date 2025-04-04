#include "TebexPlugin.h"

#include "Engine/Engine.h"
#include "TebexPluginAPI.h"
#include "TebexUE5.h"
#include "Plugin/Models/Commands.h"
#include "Plugin/Models/DuePlayers.h"
#include "Plugin/Models/JoinEvent.h"
#include "Interfaces/IHttpResponse.h"
#include "Util/SharedAliases.h"

FServerInformation UTebexPlugin::ServerInformation;
FJoinEvents UTebexPlugin::JoinEvents;
TArray<FCommand> UTebexPlugin::CompletedCommands;

bool UTebexPlugin::Enable()
{
	BeginRepeatingTask(60 * 30, []
	{
		RefreshServerInformation(nullptr);
	});

	BeginRepeatingTask(120, []
	{
		PerformCheck(nullptr);
	});

	BeginRepeatingTask(60 * 5, []
	{
		SendJoinEvents(nullptr);
		DeleteCompletedCommands(nullptr);
	});

	return true;
}

void UTebexPlugin::PerformCheck(TOkCallable<FEmptyBody> Callback)
{
    // Get due players
    UTebexPluginAPI::GetDuePlayers([Callback](FDuePlayersResponse DuePlayers)
    {
        // Counter to track how many asynchronous tasks are pending
        TAtomic<int32> PendingOperations = DuePlayers.players.Num(); // Start with the number of players

        // If no players, invoke callback straight away
        if (PendingOperations == 0)
        {
            if (Callback != nullptr)
            {
                Callback(FTebexUE5::EmptyRequest);
            }
            return;
        }

        // For each player, fetch commands
        for (FDuePlayer Player : DuePlayers.players)
        {
            UTebexPluginAPI::GetOnlineCommands(Player.uuid, [&PendingOperations, Callback](FOnlineCommandsResponse OnlineCommands)
            {
                // Process online commands for the given player
                for (FCommand Command : OnlineCommands.commands)
                {
                    if (Command.conditions.delay > 0)
                    {
                        DelayCommand(Command, Command.conditions.delay);
                        continue;
                    }

                    bool success = DispatchCommand(Command.command);
                    if (!success)
                    {
                        FTebexUE5::Log("Failed to execute command: '" + FString::FromInt(Command.id) + "'/" + Command.command);
                        continue;
                    }
                    RecordCompletedCommand(Command);
                }

                // Decrement the pending operations counter
                --PendingOperations;

                // If all commands have been processed, invoke the callback
                if (PendingOperations == 0 && Callback != nullptr)
                {
                    Callback(FTebexUE5::EmptyRequest);
                }

            }, [&PendingOperations, Callback](FHttpErr OnlineCommandsFailResponse)
            {
                // Log failure for fetching the online commands
                FTebexUE5::Log("Failed to get online commands: " + OnlineCommandsFailResponse->GetContentAsString());

                // Decrement the pending operations counter on failure
                --PendingOperations;

                // If all commands have been processed or failed, invoke the callback
                if (PendingOperations == 0 && Callback != nullptr)
                {
                    Callback(FTebexUE5::EmptyRequest);
                }
            });
        }
    }, [Callback](FHttpErr DuePlayersFailResponse)
    {
        // Failed to get the due players, log and trigger the callback
        FTebexUE5::Log("Failed to get due players: " + DuePlayersFailResponse->GetContentAsString());
        
        if (Callback != nullptr)
        {
            Callback(FTebexUE5::EmptyRequest);
        }
    });
}

void UTebexPlugin::Shutdown()
{
	SendJoinEvents(nullptr);
	DeleteCompletedCommands(nullptr);
}

void UTebexPlugin::RefreshServerInformation(TOkCallable<FServerInformation> Callback)
{
	UTebexPluginAPI::GetServerInformation([Callback](const FServerInformation& RemoteServerInformation)
	{
		ServerInformation = RemoteServerInformation;
		if (Callback != nullptr)
		{
			Callback(ServerInformation);	
		}
	}, [Callback](FHttpErr ServerInformationFailResponse)
	{
		FTebexUE5::Log("Failed to get server information: " + ServerInformationFailResponse->GetContentAsString());
		Callback(FServerInformation());
	});
}

void UTebexPlugin::SendJoinEvents(TOkCallable<FEmptyBody> Callback)
{
	UTebexPluginAPI::SendEvents(JoinEvents, [Callback](const FEmptyBody& Response)
	{
		JoinEvents.events.Empty();
		if (Callback != nullptr)
		{
			Callback(Response);	
		}
	}, [](FHttpErr Response)
	{
		FTebexUE5::Log("Failed to send join events: " + Response->GetContentAsString());
	});
}

bool UTebexPlugin::DispatchCommand(const FString Command)
{
	//TODO send to engine command processor
	return false;
}

void UTebexPlugin::DelayCommand(const FCommand Command, int32 Seconds)
{
	BeginDelayedTask(Command.conditions.delay, [Command]()
	{
		DispatchCommand(Command.command);
	});
}

void UTebexPlugin::RecordJoinEvent(FString PlayerId, FString PlayerUsername)
{
	FJoinEvent Event;
	Event.username_id = PlayerId; // a non-changing player unique numeric/uuid identifier
	Event.username = PlayerUsername; // your game's "friendly" username implementation
	Event.event_date = ""; //TODO
	Event.event_type = "server.join";
	
	JoinEvents.events.Add(Event);
}

void UTebexPlugin::RecordCompletedCommand(const FCommand Command)
{
	CompletedCommands.Add(Command);
}

void UTebexPlugin::DeleteCompletedCommands(TOkCallable<FEmptyBody> Callback)
{
	TArray<int32> IDsToDelete;
	for (FCommand Command : CompletedCommands)
	{
		IDsToDelete.Add(Command.id);
	}
	
	UTebexPluginAPI::DeleteCommands(IDsToDelete, [Callback](const FEmptyBody& Response)
	{
		CompletedCommands.Empty();
		if (Callback != nullptr)
		{
			Callback(Response);	
		}
	}, [Callback](FHttpErr Response)
	{
		FTebexUE5::Log("Failed to send delete commands: " + Response->GetContentAsString());
		Callback(FTebexUE5::EmptyRequest);
	});
}

void UTebexPlugin::BeginRepeatingTask(int IntervalSeconds, TFunction<void()> Task)
{
	if (IntervalSeconds <= 0)
	{
		UE_LOG(LogTebexPlugin, Warning, TEXT("Invalid interval for BeginRepeatingTask: %d seconds"), IntervalSeconds);
		return;
	}

	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([Task = MoveTemp(Task)](float DeltaTime) -> bool
		{
			Task(); // execute the task every time the ticker is called
			return true; // indicate we repeat the task
		}),
		static_cast<float>(IntervalSeconds)
	);
}

void UTebexPlugin::BeginDelayedTask(int DelaySeconds, TFunction<void()> Task)
{
	if (DelaySeconds < 0)
	{
		UE_LOG(LogTebexPlugin, Warning, TEXT("Invalid delay for BeginDelayedTask: %d seconds"), DelaySeconds);
		return;
	}

	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([Task = MoveTemp(Task)](float DeltaTime) -> bool
		{
			Task();
			return false; // Task runs only once
		}),
		static_cast<float>(DelaySeconds)
	);
}