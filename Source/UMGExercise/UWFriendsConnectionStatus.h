#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWFriendsConnectionStatus.generated.h"


class AMVFriendsConnectionStatus;
class UUWFriendsStatusList;

/**
 * Widget containing the two lists to display the players friends connection status.
 */
UCLASS()
class UMGEXERCISE_API UUWFriendsConnectionStatus : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Setup(AMVFriendsConnectionStatus* ModelView);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUWFriendsStatusList* FriendsStatusListConnected;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUWFriendsStatusList* FriendsStatusListDisconnected;
	
private:

	UPROPERTY(VisibleAnywhere)
	AMVFriendsConnectionStatus* ModelViewInstance;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnFriendConnected(const FRemotePlayerStatus& FriendStatus);
	UFUNCTION()
	void OnFriendDisconnected(const FRemotePlayerStatus& FriendStatus);
	UFUNCTION()
	void OnFriendsStatusPopulated();

	/* Take the connected and disconnect player friends data and fill the respective lists
	with entries to represent this data. */
	void PopulateListsWithCurrentData();
};
