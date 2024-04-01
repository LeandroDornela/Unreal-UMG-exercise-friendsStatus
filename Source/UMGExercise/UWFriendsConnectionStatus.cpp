
#include "UWFriendsConnectionStatus.h"
#include "UWFriendsStatusList.h"
#include "MVFriendsConnectionStatus.h"
#include "DMFriendsConnectionStatus.h"


void UUWFriendsConnectionStatus::Setup(AMVFriendsConnectionStatus* ModelView)
{
	if (!IsValid(ModelView))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Model View."));
		return;
	}

	ModelViewInstance = ModelView;

	ModelViewInstance->OnFriendConnectedDelegate.AddUniqueDynamic(this, &UUWFriendsConnectionStatus::OnFriendConnected);
	ModelViewInstance->OnFriendDisconnectedDelegate.AddUniqueDynamic(this, &UUWFriendsConnectionStatus::OnFriendDisconnected);
	ModelViewInstance->OnFriendsStatusPopulatedDelegate.AddUniqueDynamic(this, &UUWFriendsConnectionStatus::OnFriendsStatusPopulated);
}


void UUWFriendsConnectionStatus::NativeConstruct()
{
	Super::NativeConstruct();

	// Populate the two lists with current friend status data. If no data is present at this moment the lists will be empty.
	PopulateListsWithCurrentData();
}


void UUWFriendsConnectionStatus::OnFriendConnected(const FRemotePlayerStatus& FriendStatus)
{
	if (!IsValid(FriendsStatusListConnected) || !IsValid(FriendsStatusListDisconnected))
	{
		UE_LOG(LogTemp, Error, TEXT("Friends connection status list not binded."));
		return;
	}


	if (!FriendsStatusListDisconnected->RemoveEntry(FriendStatus.PlayerId))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to remove friend %s from Connected friends list."), *FriendStatus.PlayerId.ToString());
	}

	FriendsStatusListConnected->AddEntry(FriendStatus);
}


void UUWFriendsConnectionStatus::OnFriendDisconnected(const FRemotePlayerStatus& FriendStatus)
{
	if (!IsValid(FriendsStatusListConnected) || !IsValid(FriendsStatusListDisconnected))
	{
		UE_LOG(LogTemp, Error, TEXT("Friends connection status list not binded."));
		return;
	}


	if (!FriendsStatusListConnected->RemoveEntry(FriendStatus.PlayerId))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to remove friend %s from Disconnected friends list."), *FriendStatus.PlayerId.ToString());
	}

	FriendsStatusListDisconnected->AddEntry(FriendStatus);
}


void UUWFriendsConnectionStatus::OnFriendsStatusPopulated()
{
	PopulateListsWithCurrentData();
}


void UUWFriendsConnectionStatus::PopulateListsWithCurrentData()
{
	if (!IsValid(ModelViewInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Model View is undefined."));
		return;
	}

	// Populate Connected friends list.
	if (IsValid(FriendsStatusListConnected))
	{
		TArray<FRemotePlayerStatus> ConnectedFriends;
		if (ModelViewInstance->GetConnectedFriendsStatus(ConnectedFriends))
		{
			FriendsStatusListConnected->PopulateList(ConnectedFriends);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Connected friends list is not binded."));
	}

	// Populate Disconnected friends list.
	if (IsValid(FriendsStatusListDisconnected))
	{
		TArray<FRemotePlayerStatus> DisconnectedFriends;
		if (ModelViewInstance->GetDisconnectedFriendsStatus(DisconnectedFriends))
		{
			FriendsStatusListDisconnected->PopulateList(DisconnectedFriends);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Disconnected friends list is not binded."));
	}
}
