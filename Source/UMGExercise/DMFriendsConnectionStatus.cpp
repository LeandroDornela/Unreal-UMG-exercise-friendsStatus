
#include "DMFriendsConnectionStatus.h"


ADMFriendsConnectionStatus::ADMFriendsConnectionStatus()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ADMFriendsConnectionStatus::Setup(const TArray<FRemotePlayerStatus>& FriendsStatus)
{
	PopulateFriendsConnectionStatus(FriendsStatus);

	bIsInitialized = true;
}


void ADMFriendsConnectionStatus::AddNewFriend(const FName& FriendId, const FRemotePlayerStatus& FriendStatus)
{
	if (!CheckIsInitialized())
	{
		return;
	}

	if (FriendsConnectionStatus.Find(FriendId))
	{
		UE_LOG(LogTemp, Error, TEXT("Can't add Player with id %s to friends. It's already registered as friend."), *FriendId.ToString());
		return;
	}

	// Add the new friend.
	FriendsConnectionStatus.Add(FriendId, FriendStatus);

	// Trigger "Event"
	OnNewFriendAddedDelegate.Broadcast(FriendStatus);
}


void ADMFriendsConnectionStatus::RemoveFriend(const FName& FriendId)
{
	if (!CheckIsInitialized())
	{
		return;
	}

	// Check if the friend exists.
	if (!FriendsConnectionStatus.Find(FriendId))
	{
		UE_LOG(LogTemp, Error, TEXT("Friend not found. Id: %s"), *FriendId.ToString());
		return;
	}

	// Remove the friend.
	FRemotePlayerStatus RemovedFriend;
	FriendsConnectionStatus.RemoveAndCopyValue(FriendId, RemovedFriend);

	// Trigger "Event"
	OnFriendRemovedDelegate.Broadcast(RemovedFriend);
}


void ADMFriendsConnectionStatus::SetFriendConnectionStatus(const FName& FriendId, const bool& bValue, const FDateTime& UpdateTime)
{
	if (!CheckIsInitialized())
	{
		return;
	}

	// Check if the friend exists.
	if (!FriendsConnectionStatus.Find(FriendId))
	{
		UE_LOG(LogTemp, Error, TEXT("Friend not found. Id: %s"), *FriendId.ToString());
		return;
	}

	if (FriendsConnectionStatus[FriendId].bIsConnected == bValue)
	{
		// Nothing to change.
		return;
	}

	// Set the new connected status.
	FriendsConnectionStatus[FriendId].bIsConnected = bValue;
	FriendsConnectionStatus[FriendId].LastConnectionSttsChange = UpdateTime;

	// Trigger the "Events"
	if (bValue) // IsConnected true.
	{
		OnFriendConnectedDelegate.Broadcast(FriendsConnectionStatus[FriendId]);
	}
	else
	{
		OnFriendDisconnectedDelegate.Broadcast(FriendsConnectionStatus[FriendId]);
	}
}


void ADMFriendsConnectionStatus::SetFriendLevel(const FName& FriendId, const int32& Value)
{
	if (!CheckIsInitialized())
	{
		return;
	}

	// Check if the friend exists.
	if (!FriendsConnectionStatus.Find(FriendId))
	{
		UE_LOG(LogTemp, Error, TEXT("Friend not found. Id: %s"), *FriendId.ToString());
		return;
	}

	if (FriendsConnectionStatus[FriendId].Level == Value)
	{
		// Nothing to change.
		return;
	}

	// Change the property.
	FriendsConnectionStatus[FriendId].Level = Value;

	// Trigger the Level change Event. Send just the value not the "real" Data itself.
	OnFriendLevelUpdateDelegate.Broadcast(FriendsConnectionStatus[FriendId]);
}


bool ADMFriendsConnectionStatus::GetFriendStatus(const FName& FriendId, FRemotePlayerStatus& OutFriendStatus) const
{
	if (!CheckIsInitialized())
	{
		UE_LOG(LogTemp, Error, TEXT("Friend not found. Id: %s"), *FriendId.ToString());
		return false;
	}

	// Check if the friend exists.
	if (!FriendsConnectionStatus.Find(FriendId))
	{
		return false;
	}

	OutFriendStatus = FriendsConnectionStatus[FriendId];

	return true;
}


inline bool ADMFriendsConnectionStatus::GetAllFriendStatusAsMap(TMap<FName, FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (!CheckIsInitialized())
	{
		return false;
	}

	if (!OutFriendsStatus.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Output TMap should be empty."));
	}

	// Clean output parameter and copy from TMap to TMap.
	// No "Reserve" needed since Append do this.
	OutFriendsStatus.Empty();
	OutFriendsStatus.Append(FriendsConnectionStatus);

	return true;
}


bool ADMFriendsConnectionStatus::GetAllFriendStatusAsArray(TArray<FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (!CheckIsInitialized())
	{
		return false;
	}

	if (!OutFriendsStatus.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Output TArray should be empty."));
	}

	OutFriendsStatus.Empty();
	OutFriendsStatus.Reserve(FriendsConnectionStatus.Num());

	for (const auto& Pair : FriendsConnectionStatus)
	{
		OutFriendsStatus.Add(Pair.Value);
	}

	return true;
}


bool ADMFriendsConnectionStatus::GetConnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (!CheckIsInitialized())
	{
		return false;
	}

	if (!OutFriendsStatus.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Output TArray should be empty."));
	}

	// Clean the referenced array.
	OutFriendsStatus.Empty();
	// Reserve space for the worst case.
	OutFriendsStatus.Reserve(FriendsConnectionStatus.Num());

	for (const auto& Pair : FriendsConnectionStatus)
	{
		if (Pair.Value.bIsConnected)
		{
			OutFriendsStatus.Add(Pair.Value);
		}
	}

	return true;
}


bool ADMFriendsConnectionStatus::GetDisconnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const
{
	if (!CheckIsInitialized())
	{
		return false;
	}

	if (!OutFriendsStatus.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Output TArray should be empty."));
	}

	// Clean the referenced array.
	OutFriendsStatus.Empty();
	// Reserve space for the worst case.
	OutFriendsStatus.Reserve(FriendsConnectionStatus.Num());

	for (const auto& Pair : FriendsConnectionStatus)
	{
		if (!Pair.Value.bIsConnected)
		{
			OutFriendsStatus.Add(Pair.Value);
		}
	}

	return true;
}


void ADMFriendsConnectionStatus::PopulateFriendsConnectionStatus(const TArray<FRemotePlayerStatus>& FriendsStatus)
{
	// A empty FriendsStatus can be valid since the player can have no friends.

	FriendsConnectionStatus.Empty();
	FriendsConnectionStatus.Reserve(FriendsStatus.Num());

	for (const auto& FriendData : FriendsStatus)
	{
		FriendsConnectionStatus.Emplace(FriendData.PlayerId, FriendData);
	}

	OnFriendsStatusPopulatedDelegate.Broadcast();
}


bool ADMFriendsConnectionStatus::CheckIsInitialized() const
{
	if (bIsInitialized)
	{
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Connection Status Data Manager is not initialized."));
	return false;
}