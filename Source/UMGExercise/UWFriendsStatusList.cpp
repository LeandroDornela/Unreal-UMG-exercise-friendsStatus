
#include "UWFriendsStatusList.h"
#include "UWFriendStatus.h"
#include "DMFriendsConnectionStatus.h" // TODO: put the struct in a .h to avoid this include
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "RemotePlayerStatusWrapper.h"


void UUWFriendsStatusList::PopulateList(const TArray<FRemotePlayerStatus>& FriendsStatus)
{
	if (!IsValid(ListViewFriendsStatus))
	{
		UE_LOG(LogTemp, Error, TEXT("List View is undefined."));
		return;
	}

	// Wrap the list.
	TArray<UObject*> WrappedStatus;
	WrappedStatus.Reserve(FriendsStatus.Num());
	for (auto Status : FriendsStatus)
	{
		URemotePlayerStatusWrapper* WrappedStatusEntry = NewObject<URemotePlayerStatusWrapper>();
		WrappedStatusEntry->RemotePlayerStatus = Status;
		WrappedStatus.Add(WrappedStatusEntry);

		ListViewEntries.Add(Status.PlayerId, WrappedStatusEntry);
	}

	// Fill the List View with all friend status.
	ListViewFriendsStatus->SetListItems(WrappedStatus);
}


void UUWFriendsStatusList::AddEntry(const FRemotePlayerStatus& FriendsStatus)
{
	if (!IsValid(ListViewFriendsStatus))
	{
		UE_LOG(LogTemp, Error, TEXT("List View is undefined."));
		return;
	}

	// Wrap the new entry.
	URemotePlayerStatusWrapper* WrappedStatusEntry = NewObject<URemotePlayerStatusWrapper>();
	WrappedStatusEntry->RemotePlayerStatus = FriendsStatus;

	// Add to wrapper entries and to list view.
	ListViewEntries.Add(FriendsStatus.PlayerId, WrappedStatusEntry);
	ListViewFriendsStatus->AddItem(WrappedStatusEntry);
}


bool UUWFriendsStatusList::RemoveEntry(const FName& PlayerId)
{
	if (!IsValid(ListViewFriendsStatus))
	{
		UE_LOG(LogTemp, Error, TEXT("List View is undefined."));
		return false;
	}

	if (!ListViewEntries.Find(PlayerId))
	{
		return false;
	}

	URemotePlayerStatusWrapper* RemovedEntry;
	if (ListViewEntries.RemoveAndCopyValue(PlayerId, RemovedEntry))
	{
		ListViewFriendsStatus->RemoveItem(RemovedEntry);
		return true;
	}

	return false;
}


void UUWFriendsStatusList::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(TextListTitle))
	{
		TextListTitle->SetText(ListTitleTextString);
	}
}
