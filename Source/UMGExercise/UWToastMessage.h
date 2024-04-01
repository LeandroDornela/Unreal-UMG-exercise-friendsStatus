
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DMFriendsConnectionStatus.h"
#include "UWToastMessage.generated.h"


// NOTE: To handle more connection popups will need to create a manager to instantiate new widgets if two or more
// players connect in a period of time smaller than the widget lifetime combined with hide animation time.
// NOTE: Can be generalized for reusable toast message.And be even more robust with a toast messages manager.

DECLARE_DYNAMIC_MULTICAST_DELEGATE(F_OnShowSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(F_OnHideSignature);

/**
 * Widget for display player friend connection notification.
 */
UCLASS()
class UMGEXERCISE_API UUWToastMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_OnShowSignature OnShowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	F_OnHideSignature OnHideDelegate;

	UFUNCTION(BlueprintCallable)
	void Setup(class AMVFriendsConnectionStatus* ModelView);

protected:

	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 1;

	UPROPERTY(BlueprintReadOnly)
	FRemotePlayerStatus CurrentShowingPlayerStatus;

	virtual void NativeConstruct() override;

	/** Show the popup triggering Hide method timer and On Show Delegate. */
	UFUNCTION(BlueprintCallable)
	void Show(const FRemotePlayerStatus& FriendStatus);

	/** If the popup is active start the hiding by "triggering the hide event" i.e. Broadcasting the Delegate. */
	UFUNCTION(BlueprintCallable)
	void Hide();
	
	UFUNCTION()
	void OnFriendConnected(const FRemotePlayerStatus& FriendStatus);

private:

	AMVFriendsConnectionStatus* ModelViewInstance;
	FTimerHandle TimerHandle;
	UWorld* World;
	bool bIsActive = false;
};
