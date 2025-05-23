#include "TebexUE5.h"
#include "CoreMinimal.h"
#include "TebexHeadlessAPI.h"
#include "TebexPluginAPI.h"
#include "Interfaces/IHttpResponse.h"
#include "Util/SharedAliases.h"

#define LOCTEXT_NAMESPACE "FTebexUE5Module"

const FEmptyBody FTebexUE5::EmptyRequest;
const FString ExamplePublicToken = TEXT("<PUBLICTOKEN>");
void FTebexUE5::StartupModule()
{
	Log("Tebex is loading...");
	UTebexHeadlessAPI::SetPublicToken(ExamplePublicToken);
	UTebexHeadlessAPI::GetWebstore([](const FWebstore& Store)
	{
		Log("Store loaded: " + Store.Name);
		UTebexHeadlessAPI::GetAllPackages([](const FPackagesResponse& Packages)
		{
			Log(FString::FromInt(Packages.data.Num()) + " packages loaded");
			// Use Packages here
		},
		[](FHttpErr GetPackagesError)
		{
			Log("Failed to get packages!");
			Log(GetPackagesError->GetContentAsString());
		});
	},
	[](FHttpErr GetWebstoreError)
	{
		Log("Failed to load webstore!");
		Log(GetWebstoreError->GetContentAsString());
	});
}

void FTebexUE5::ShutdownModule()
{
	
}

void FTebexUE5::Log(const FString& Message)
{
	UE_LOG(LogTebex, Display, TEXT("%s"), *Message);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTebexUE5, TebexUE5)