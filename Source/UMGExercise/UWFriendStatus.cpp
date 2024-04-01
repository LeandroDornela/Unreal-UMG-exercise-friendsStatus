
#include "UWFriendStatus.h"
#include "Components/TextBlock.h"
#include "RemotePlayerStatusWrapper.h"

void UUWFriendStatus::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	URemotePlayerStatusWrapper* Entry = Cast<URemotePlayerStatusWrapper>(ListItemObject);

	if (!IsValid(Entry))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to cast UObject to URemotePlayerStatusWrapper."));
		return;
	}

	if (IsValid(Nickname))
	{
		Nickname->SetText(FText::FromString(Entry->RemotePlayerStatus.Nickname));
	}
	
	if (IsValid(Level))
	{
		Level->SetText(FText::FromString(FString::FromInt(Entry->RemotePlayerStatus.Level)));
	}

	// Store current player status data.
	PlayerStatus = Entry->RemotePlayerStatus;
}


FRemotePlayerStatus UUWFriendStatus::GetPlayerStatus()
{
	return PlayerStatus;
}
