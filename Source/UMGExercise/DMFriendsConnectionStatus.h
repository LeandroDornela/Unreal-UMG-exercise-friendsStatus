
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DMFriendsConnectionStatus.generated.h"


/**
 * Struct for storing a remote player status data.
 * TODO: Move to separated .h.I tried this bus was having problems, look like Unreal don't like Structs on separated headers.
 * Starting by the fact that don't have a option to create it from Editor9as far as I know).
 */
USTRUCT(BlueprintType)
struct UMGEXERCISE_API FRemotePlayerStatus
{
	GENERATED_USTRUCT_BODY()

public:

	/** Unique generated player ID. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerId;

	/** User defined nickname. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Nickname;

	/** Current player level. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsConnected;

	/** The last time the player connection status changed.
	If is connected is when was connection, if is disconnected is when was the disconnection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime LastConnectionSttsChange;


	FRemotePlayerStatus()
		: PlayerId("")
		, Nickname("Player Name")
		, Level(0)
		, bIsConnected(false)
		, LastConnectionSttsChange(FDateTime::Now())
	{}


	FRemotePlayerStatus(FName _PlayerId, FString _Nickname, int32 _Level, bool _bIsConnected, FDateTime _LastConnectionSttsChange)
		: PlayerId(_PlayerId)
		, Nickname(_Nickname)
		, Level(_Level)
		, bIsConnected(_bIsConnected)
		, LastConnectionSttsChange(_LastConnectionSttsChange)
	{}
};


// Pass as const ref to avoid modification and copying.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewFriendAddedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendRemovedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendConnectedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendDisconnectedSignature, const FRemotePlayerStatus&, FriendStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendLevelUpdateSignature, const FRemotePlayerStatus&, FriendStatus);
// Avoid return all the data, listeners can request it if needed.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendsStatusPopulatedSignature);


/**
 * DM initials for Data Manager. Control the access and manipulation of Player Friends Connection Status data.
 */
UCLASS()
class UMGEXERCISE_API ADMFriendsConnectionStatus : public AActor
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnNewFriendAddedSignature OnNewFriendAddedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFriendRemovedSignature OnFriendRemovedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFriendConnectedSignature OnFriendConnectedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFriendDisconnectedSignature OnFriendDisconnectedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFriendLevelUpdateSignature OnFriendLevelUpdateDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFriendsStatusPopulatedSignature OnFriendsStatusPopulatedDelegate;


	ADMFriendsConnectionStatus();

	/** Fill the data table. Can be with external data, like received in a json file. */
	UFUNCTION(BlueprintCallable)
	void Setup(const TArray<FRemotePlayerStatus>& FriendsStatus);

	UFUNCTION(BlueprintCallable)
	void AddNewFriend(const FName& FriendId, const FRemotePlayerStatus& FriendStatus);

	UFUNCTION(BlueprintCallable)
	void RemoveFriend(const FName& FriendId);
	
	UFUNCTION(BlueprintCallable)
	void SetFriendConnectionStatus(const FName& FriendId, const bool& bValue, const FDateTime& UpdateTime);

	UFUNCTION(BlueprintCallable)
	void SetFriendLevel(const FName& FriendId, const int32& Value);

	// NOTE: "const" so a get will not change any parameter of this class.
	UFUNCTION(BlueprintCallable)
	bool GetFriendStatus(const FName& FriendId, FRemotePlayerStatus& OutFriendStatus) const;

	// NOTE: UFUNCTION Overload is not possible. Renamed GetAllFriendStatu to have the return type as suffix.
	UFUNCTION(BlueprintCallable)
	bool GetAllFriendStatusAsMap(TMap<FName, FRemotePlayerStatus>& OutFriendsStatus) const;

	UFUNCTION(BlueprintCallable)
	bool GetAllFriendStatusAsArray(TArray<FRemotePlayerStatus>& OutFriendsStatus) const;

	UFUNCTION(BlueprintCallable)
	bool GetConnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const;

	UFUNCTION(BlueprintCallable)
	bool GetDisconnectedFriendsStatus(TArray<FRemotePlayerStatus>& OutFriendsStatus) const;


private:
	/**
	 * The Friend Id is the Key. Runtime friends connection data.
	 * TODO : For now, I don't see the need to store it in another place since this data will not be saved.
	 * Maybe have this in another class for between levels persistence if this manager is not persisted.
	*/
	UPROPERTY(VisibleAnywhere, Category = "View Only")
	TMap<FName, FRemotePlayerStatus> FriendsConnectionStatus;

	UPROPERTY(VisibleAnywhere, Category = "View Only")
	bool bIsInitialized;


	/** Populate the FriendsConnectionStatus TMap with a TArray Data calling OnFriendsStatusPopulatedDelegate at end. */
	void PopulateFriendsConnectionStatus(const TArray<FRemotePlayerStatus>& FriendsStatus);

	/** Return true when the Data MAnager is initialized, false cc with a error message. */
	bool CheckIsInitialized() const;
};
