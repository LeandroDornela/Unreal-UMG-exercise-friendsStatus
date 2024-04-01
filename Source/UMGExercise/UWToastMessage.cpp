
#include "UWToastMessage.h"
#include "Components/TextBlock.h"
#include "MVFriendsConnectionStatus.h"


void UUWToastMessage::Setup(AMVFriendsConnectionStatus* ModelView)
{
	if (!IsValid(ModelView))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Model View."));
		return;
	}

	ModelViewInstance = ModelView;

	World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("World is undefined."));
		return;
	}

	ModelViewInstance->OnFriendConnectedDelegate.AddUniqueDynamic(this, &UUWToastMessage::OnFriendConnected);
}


void UUWToastMessage::Show(const FRemotePlayerStatus& FriendStatus)
{
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("World is undefined."));
		return;
	}

	CurrentShowingPlayerStatus = FriendStatus;

	// Set timer with Hide widget method callback.
	World->GetTimerManager().SetTimer(TimerHandle, this, &UUWToastMessage::Hide, LifeTime, false);

	this->SetVisibility(ESlateVisibility::Visible);
	bIsActive = true;

	OnShowDelegate.Broadcast();
}


void UUWToastMessage::Hide()
{
	if (!bIsActive)
	{
		// Already inactive.
		return;
	}

	bIsActive = false;

	OnHideDelegate.Broadcast();
}


void UUWToastMessage::NativeConstruct()
{
	Super::NativeConstruct();

	// Start hidden.
	this->SetVisibility(ESlateVisibility::Hidden);
}


void UUWToastMessage::OnFriendConnected(const FRemotePlayerStatus& FriendStatus)
{
	Show(FriendStatus);
}
