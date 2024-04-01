
#pragma once

#include "CoreMinimal.h"
#include "UMGExerciseGameMode.h"
#include "GMConnectionStatusTesting.generated.h"

class AMVFriendsConnectionStatus;
class ADMFriendsConnectionStatus;
class UUWFriendsConnectionStatus;
class UUWToastMessage;

/**
 * Structure for default testing data stored in Data Tables.
 */
USTRUCT(BlueprintType)
struct FPlayerFriendStatusTesting : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsConnected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime LastConnectionSttsChange;


	FPlayerFriendStatusTesting()
		: Nickname("Player Name")
		, Level(0)
		, bIsConnected(false)
		, LastConnectionSttsChange(FDateTime::Now())
	{}

	FPlayerFriendStatusTesting(FString _Nickname, int32 _Level, bool _bIsConnected, FDateTime _LastConnectionSttsChange)
		: Nickname(_Nickname)
		, Level(_Level)
		, bIsConnected(_bIsConnected)
		, LastConnectionSttsChange(_LastConnectionSttsChange)
	{}
};


/**
 * Game Mode for testing player friends connection status manipulation and related UI.
 */
UCLASS()
class UMGEXERCISE_API AGMConnectionStatusTesting : public AUMGExerciseGameMode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* FriendsConnectionStatusDT;

	UPROPERTY(EditDefaultsOnly, Category = "Blueprints")
	TSubclassOf<AMVFriendsConnectionStatus> MVFriendConnectionStatusBPClass;

	UPROPERTY(EditDefaultsOnly, Category = "Blueprints")
	TSubclassOf<ADMFriendsConnectionStatus> DMFriendConnectionStatusBPClass;

	UPROPERTY(EditDefaultsOnly, Category = "Blueprints")
	TSubclassOf<UUWFriendsConnectionStatus> UWFriendConnectionStatusBPClass;

	UPROPERTY(EditDefaultsOnly, Category = "Blueprints")
	TSubclassOf<UUWToastMessage> UWFriendConnectionMsgBPClass;

	// (Des)Connection simulation
	UPROPERTY(EditAnywhere, Category = "Testing")
	float ConnectionSimInterval = 0;
	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, Category = "View Only")
	AMVFriendsConnectionStatus* MVFriendConnectionstatusInstance;

	UPROPERTY(VisibleAnywhere, Category = "View Only")
	ADMFriendsConnectionStatus* DMFriendConnectionStatusInstance;

	// UPROPERTY(VisibleAnywhere, Category = "View Only") This will pollute the details view of the Game Mode.
	UUWFriendsConnectionStatus* UWFriendConnectionStatusInstance;

	UUWToastMessage* UWFriendConnectionMsgInstance;


	virtual void BeginPlay() override;

	/** Just spawns the data manager. */
	void SpawnDataManager(UWorld* World);

	/** Just spawns the model view. */
	void SpawnModelView(UWorld* World);

	/** Initialize the Data Manager whit the testing data. */
	void InitializeDataManager();

	/** Initialize the Model View providing the Data Manager reference.*/
	void InitializeFriendsConStatusModelView();

	/** Instantiate Friend connection toast message. */
	void SpawnToastMessageWidget(UWorld* World);

	/** Instantiate Friends connection status lists. */
	void SpawnStatusLitsWidget(UWorld* World);

	/** Just set a random player from testing data base to be connected or disconnected. */
	void SimulateConnection();

	/** Set the mouse cursor to be enable and free or not*/
	void EnableMouse(UWorld* World, bool state);
};
