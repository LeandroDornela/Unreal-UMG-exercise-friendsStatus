
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DMFriendsConnectionStatus.h"
#include "RemotePlayerStatusWrapper.generated.h"

struct FRemotePlayerStatus;

/**
 * To use View Lists we need to pass our objects data to create entries as a UObject. This Wrapper is just to pass the
 * data without needing to change the FRemotePlayerStatus structure and all related implementation because of a
 * UI element related restriction.
 */
UCLASS(Blueprintable)
class UMGEXERCISE_API URemotePlayerStatusWrapper : public UObject
{
	GENERATED_BODY()
	
public:
	URemotePlayerStatusWrapper() {}

	// Setting the property as const is not allowed, so to avoid modification of the Player status on UI classes,
	// pass it as the value.
	URemotePlayerStatusWrapper(FRemotePlayerStatus PlayerStatus)
		: RemotePlayerStatus(PlayerStatus) {}


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRemotePlayerStatus RemotePlayerStatus;
};
