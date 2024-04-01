
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MVFriendsConnectionStatus.generated.h"

class ADMFriendsConnectionStatus;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MV_OnNewFriendAddedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MV_OnFriendRemovedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MV_OnFriendConnectedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MV_OnFriendDisconnectedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MV_OnFriendLevelUpdateSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(F_MV_OnFriendsStatusPopulatedSignature);


/**
 * MV initials for Model View. Communication between View or UI widget classes to the Game data.
 * Will also threat and validate received data and/or input from UI.
 */
UCLASS()
class UMGEXERCISE_API AMVFriendsConnectionStatus : public AActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_MV_OnNewFriendAddedSignature OnNewFriendAddedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_MV_OnFriendRemovedSignature OnFriendRemovedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_MV_OnFriendConnectedSignature OnFriendConnectedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_MV_OnFriendDisconnectedSignature OnFriendDisconnectedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_MV_OnFriendLevelUpdateSignature OnFriendLevelUpdateDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_MV_OnFriendsStatusPopulatedSignature OnFriendsStatusPopulatedDelegate;


	AMVFriendsConnectionStatus();

	UFUNCTION()
	void Setup(ADMFriendsConnectionStatus* FriendsConnectionStatusDMPtr);

	UFUNCTION(BlueprintCallable)
	bool GetAllFriendStatusAsMap(TMap<FName, FRemotePlayerStatus>& OutFriendsStatus) const;

	UFUNCTION(BlueprintCallable)
	bool GetAllFriendStatusAsArray(TArray<FRemotePlayerStatus>& OutFriendsStatus) const;

	UFUNCTION(BlueprintCallable)
	bool GetConnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const;

	UFUNCTION(BlueprintCallable)
	bool GetDisconnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const;


private:

	UPROPERTY(VisibleAnywhere, Category = "View Only")
	ADMFriendsConnectionStatus* FriendsConnectionStatusDataManager;

	UPROPERTY(VisibleAnywhere, Category = "View Only")
	bool bIsInitialized;


	UFUNCTION()
	void OnNewFriendAdded(const FRemotePlayerStatus& FriendStatus);

	UFUNCTION()
	void OnFriendRemoved(const FRemotePlayerStatus& FriendId);

	UFUNCTION()
	void OnFriendConnected(const FRemotePlayerStatus& FriendId);

	UFUNCTION()
	void OnFriendDisconnected(const FRemotePlayerStatus& FriendId);

	UFUNCTION()
	void OnFriendLevelUpdate(const FRemotePlayerStatus& FriendStatus);

	/**
	 * Update the UI on having a new data setup for friends status. Useful wen the players status data only
	 * arrive after the initial instantiation and setup of the Widget.
	 */
	UFUNCTION()
	void OnFriendsStatusPopulated();
};
