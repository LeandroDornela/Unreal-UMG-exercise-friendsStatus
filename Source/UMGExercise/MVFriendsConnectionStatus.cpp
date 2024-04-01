
#include "MVFriendsConnectionStatus.h"
#include "DMFriendsConnectionStatus.h"


AMVFriendsConnectionStatus::AMVFriendsConnectionStatus()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AMVFriendsConnectionStatus::Setup(ADMFriendsConnectionStatus* FriendsConnectionStatusDMPtr)
{
	if (!IsValid(FriendsConnectionStatusDMPtr))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Data Manager."));
		return;
	}

	FriendsConnectionStatusDataManager = FriendsConnectionStatusDMPtr;
	
	// Listening to Connection status Data manager changes.
	FriendsConnectionStatusDataManager->OnNewFriendAddedDelegate.AddUniqueDynamic(this, &AMVFriendsConnectionStatus::OnNewFriendAdded);
	FriendsConnectionStatusDataManager->OnFriendRemovedDelegate.AddUniqueDynamic(this, &AMVFriendsConnectionStatus::OnFriendRemoved);
	FriendsConnectionStatusDataManager->OnFriendConnectedDelegate.AddUniqueDynamic(this, &AMVFriendsConnectionStatus::OnFriendConnected);
	FriendsConnectionStatusDataManager->OnFriendDisconnectedDelegate.AddUniqueDynamic(this, &AMVFriendsConnectionStatus::OnFriendDisconnected);
	FriendsConnectionStatusDataManager->OnFriendLevelUpdateDelegate.AddUniqueDynamic(this, &AMVFriendsConnectionStatus::OnFriendLevelUpdate);
	FriendsConnectionStatusDataManager->OnFriendsStatusPopulatedDelegate.AddUniqueDynamic(this, &AMVFriendsConnectionStatus::OnFriendsStatusPopulated);

	bIsInitialized = true;
}


bool AMVFriendsConnectionStatus::GetAllFriendStatusAsMap(TMap<FName, FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (FriendsConnectionStatusDataManager->GetAllFriendStatusAsMap(OutFriendsStatus))
	{
		return true;
	}

	return false;
}


bool AMVFriendsConnectionStatus::GetAllFriendStatusAsArray(TArray<FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (FriendsConnectionStatusDataManager->GetAllFriendStatusAsArray(OutFriendsStatus))
	{
		return true;
	}

	return false;
}


bool AMVFriendsConnectionStatus::GetConnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (FriendsConnectionStatusDataManager->GetConnectedFriendsStatus(OutFriendsStatus))
	{
		return true;
	}

	return false;
}


bool AMVFriendsConnectionStatus::GetDisconnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (FriendsConnectionStatusDataManager->GetDisconnectedFriendsStatus(OutFriendsStatus))
	{
		return true;
	}

	return false;
}


void AMVFriendsConnectionStatus::OnNewFriendAdded(const FRemotePlayerStatus& FriendStatus)
{
	OnNewFriendAddedDelegate.Broadcast(FriendStatus);
}


void AMVFriendsConnectionStatus::OnFriendRemoved(const FRemotePlayerStatus& FriendStatus)
{
	OnFriendRemovedDelegate.Broadcast(FriendStatus);
}


void AMVFriendsConnectionStatus::OnFriendConnected(const FRemotePlayerStatus& FriendStatus)
{
	OnFriendConnectedDelegate.Broadcast(FriendStatus);
}


void AMVFriendsConnectionStatus::OnFriendDisconnected(const FRemotePlayerStatus& FriendStatus)
{
	OnFriendDisconnectedDelegate.Broadcast(FriendStatus);
}


void AMVFriendsConnectionStatus::OnFriendLevelUpdate(const FRemotePlayerStatus& FriendStatus)
{
	OnFriendLevelUpdateDelegate.Broadcast(FriendStatus);
}


void AMVFriendsConnectionStatus::OnFriendsStatusPopulated()
{
	OnFriendsStatusPopulatedDelegate.Broadcast();
}
