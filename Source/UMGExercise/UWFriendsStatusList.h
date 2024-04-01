
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWFriendsStatusList.generated.h"

struct FRemotePlayerStatus;
class URemotePlayerStatusWrapper;
class UListView;
class UTextBlock;

/**
 * Widget that contains a List View for displaying Player Friends status.
 */
UCLASS()
class UMGEXERCISE_API UUWFriendsStatusList : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Add all elements of the array to a array of wrapped FRemotePlayerStatus then add these to the List View. */
	UFUNCTION(BlueprintCallable)
	void PopulateList(const TArray<FRemotePlayerStatus>& FriendsStatus);

	/** Create a new wrapped entry and add it to the list view. */
	UFUNCTION(BlueprintCallable)
	void AddEntry(const FRemotePlayerStatus& FriendsStatus);

	/** Remove respective player from wrapped and list view. */
	UFUNCTION(BlueprintCallable)
	bool RemoveEntry(const FName& PlayerId);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UListView* ListViewFriendsStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TextListTitle;
	
	UPROPERTY(EditAnywhere, Category = "List Header")
	FText ListTitleTextString;

	virtual void NativeConstruct() override;

private:

	/** Store the pointer to the UObjects used to fill the list.Needed for example to
	remove a item from list view, what require a UObject reference. */
	TMap<FName, URemotePlayerStatusWrapper*> ListViewEntries;
};
