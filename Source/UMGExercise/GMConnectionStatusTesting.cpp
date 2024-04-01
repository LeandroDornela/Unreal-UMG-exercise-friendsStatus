
#include "GMConnectionStatusTesting.h"
#include "MVFriendsConnectionStatus.h"
#include "DMFriendsConnectionStatus.h"
#include "UWFriendsConnectionStatus.h"
#include "UWToastMessage.h"


void AGMConnectionStatusTesting::BeginPlay()
{
	Super::BeginPlay();

	// Get and check world.
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get World in Game Mode Begin Play."));
		return;
	}

	SpawnDataManager(World);
	SpawnModelView(World);
	
	InitializeDataManager();
	InitializeFriendsConStatusModelView();

	// TODO: Adding this Widget after the other make the Hit testing on this still working even when set to not testable.
	// Would be good to find another solution over changing spawn order.
	SpawnToastMessageWidget(World);
	SpawnStatusLitsWidget(World);

	// Start timer for repeatedly call the connection/disconnection simulation.
	World->GetTimerManager().SetTimer(TimerHandle, this, &AGMConnectionStatusTesting::SimulateConnection, ConnectionSimInterval, true);

	// For that demonstration, free the mouse so you can easily test the UI interaction.
	EnableMouse(World, true);
}


void AGMConnectionStatusTesting::SpawnDataManager(UWorld* World)
{
	// Spawn the data manager for friends connection status.
	if (IsValid(DMFriendConnectionStatusBPClass))
	{
		// Check if the UObject is a Blueprint to avoid instancing directly from C++ class.
		if (!DMFriendConnectionStatusBPClass->IsInBlueprint())
		{
			UE_LOG(LogTemp, Warning, TEXT("The Data Manager class was expected to be a Blueprint."));
		}

		DMFriendConnectionStatusInstance = World->SpawnActor<ADMFriendsConnectionStatus>(DMFriendConnectionStatusBPClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Data Manager Blueprint is not defined."));
	}
}


void AGMConnectionStatusTesting::SpawnModelView(UWorld* World)
{
	// Spawn the model view for friends connection status.
	if (IsValid(MVFriendConnectionStatusBPClass))
	{
		// Check if the UObject is a Blueprint to avoid instancing directly from C++ class.
		if (!MVFriendConnectionStatusBPClass->IsInBlueprint())
		{
			UE_LOG(LogTemp, Warning, TEXT("The Model View class was expected to be a Blueprint."));
		}

		MVFriendConnectionstatusInstance = World->SpawnActor<AMVFriendsConnectionStatus>(MVFriendConnectionStatusBPClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Model View Blueprint is not defined."));
	}
}


void AGMConnectionStatusTesting::InitializeDataManager()
{
	if (!IsValid(DMFriendConnectionStatusInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Dada Manager is not defined."));
		return;
	}

	if (!IsValid(FriendsConnectionStatusDT))
	{
		UE_LOG(LogTemp, Error, TEXT("Dada Table is not defined."));
		return;
	}

	// Initialize with testing data from Data Table.
	// Get Rows
	TArray<FPlayerFriendStatusTesting*> Rows;
	FriendsConnectionStatusDT->GetAllRows<FPlayerFriendStatusTesting>("", Rows);
	// Get names / players ids
	TArray<FName> RowNames = FriendsConnectionStatusDT->GetRowNames();
	
	TArray<FRemotePlayerStatus> Status;
	Status.Reserve(Rows.Num());

	for (int i = 0; i < Rows.Num(); i++)
	{
		// Create a FRemotePlayerStatus from FPlayerFriendStatusTesting and add it to Status array.
		Status.Add(FRemotePlayerStatus(RowNames[i], Rows[i]->Nickname, Rows[i]->Level, Rows[i]->bIsConnected, Rows[i]->LastConnectionSttsChange));
	}

	// Initialize data.
	DMFriendConnectionStatusInstance->Setup(Status);
}


void AGMConnectionStatusTesting::InitializeFriendsConStatusModelView()
{
	if (!IsValid(MVFriendConnectionstatusInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Model View is not defined."));
		return;
	}

	MVFriendConnectionstatusInstance->Setup(DMFriendConnectionStatusInstance);
}


void AGMConnectionStatusTesting::SpawnToastMessageWidget(UWorld* World)
{
	if (IsValid(UWFriendConnectionMsgBPClass))
	{
		UWFriendConnectionMsgInstance = CreateWidget<UUWToastMessage>(World, UWFriendConnectionMsgBPClass);
		if (IsValid(UWFriendConnectionMsgInstance))
		{
			UWFriendConnectionMsgInstance->Setup(MVFriendConnectionstatusInstance);
			UWFriendConnectionMsgInstance->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to create Widget from UWFriendConnectionMsgBPClass."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UWFriendConnectionMsgBPClass is not defined."));
	}
}


void AGMConnectionStatusTesting::SpawnStatusLitsWidget(UWorld* World)
{
	if (IsValid(UWFriendConnectionStatusBPClass))
	{
		UWFriendConnectionStatusInstance = CreateWidget<UUWFriendsConnectionStatus>(World, UWFriendConnectionStatusBPClass);
		if (IsValid(UWFriendConnectionStatusInstance))
		{
			UWFriendConnectionStatusInstance->Setup(MVFriendConnectionstatusInstance);
			UWFriendConnectionStatusInstance->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to create Widget from UWFriendConnectionStatusBPClass."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UWFriendConnectionStatusBPClass is not defined."));
	}
}


void AGMConnectionStatusTesting::SimulateConnection()
{
	if (!IsValid(DMFriendConnectionStatusInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Dada Manager is not defined."));
		return;
	}

	if (!IsValid(FriendsConnectionStatusDT))
	{
		UE_LOG(LogTemp, Error, TEXT("Dada Table is not defined."));
		return;
	}

	// NOTE: Bad performance, but enough for testing.
	TArray<FName> Names = FriendsConnectionStatusDT->GetRowNames();
	DMFriendConnectionStatusInstance->SetFriendConnectionStatus(Names[FMath::RandRange(0, Names.Num() - 1)], FMath::RandBool(), FDateTime::Now());
}


void AGMConnectionStatusTesting::EnableMouse(UWorld* World, bool State)
{
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->bShowMouseCursor = State;
		PlayerController->bEnableClickEvents = State;
		PlayerController->bEnableMouseOverEvents = State;
	}
}
