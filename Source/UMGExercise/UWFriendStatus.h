
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DMFriendsConnectionStatus.h"
#include "UWFriendStatus.generated.h"


/**
 * Widget of the List View Entry to display a Player Status data.
 */
UCLASS()
class UMGEXERCISE_API UUWFriendStatus : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Nickname;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Level;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION(BlueprintCallable)
	FRemotePlayerStatus GetPlayerStatus();

private:

	FRemotePlayerStatus PlayerStatus;
};
