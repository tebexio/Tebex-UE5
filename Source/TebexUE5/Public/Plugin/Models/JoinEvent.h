#pragma once

#include "CoreMinimal.h"
#include "JoinEvent.generated.h"

USTRUCT()
struct FJoinEvent
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString username_id;

	UPROPERTY()
	FString username;
	
	UPROPERTY()
	FString ip;

	UPROPERTY()
	FString event_date;

	UPROPERTY()
	FString event_type;

	FJoinEvent() : username_id(""), username(""), ip(""), event_date(""), event_type("") {}

	FJoinEvent(FString username_id, FString username, FString ip) : username_id(username_id), username(username), ip(ip), event_date(FDateTime::UtcNow().ToString(TEXT("%Y-%m-%d %H:%M:%S"))), event_type("server.join") {}
};

USTRUCT()
struct FJoinEvents
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FJoinEvent> events;

	FJoinEvents() : events() {}
};