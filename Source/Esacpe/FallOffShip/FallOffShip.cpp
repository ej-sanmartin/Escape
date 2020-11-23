#include "FallOffShip.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Pawn.h"
#include "Math/TransformNonVectorized.h"
#include "TimerManager.h"

UFallOffShip::UFallOffShip()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFallOffShip::BeginPlay()
{
	Super::BeginPlay();

	DebugPlayerControllerSearchVerification();
	SetupFallZones();
	SetupSpawnLocations();
}

void UFallOffShip::DebugPlayerControllerSearchVerification()
{
	if(!GetPlayerPawnController())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find player controller"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Found player controller"));
	}
}

void UFallOffShip::SetupFallZones()
{
	FallZones[0] = OriginFallZone.Get();
	FallZones[1] = OtherFallZone.Get();
}

void UFallOffShip::SetupSpawnLocations()
{
	RespawnPlayerStart = PlayerStartSpawnPoint.Get();

	if(GetOwner()->Tags[0] == "Player")
	{
		OwnerRespawnPoint = RespawnPlayerStart->GetTransform().GetTranslation();
	}

	if(OwnerRespawnPoint == DefaultRespawnPoint)
	{
		UE_LOG(LogTemp, Error, TEXT("FVector Respawn Point for item attached to this script not set for %s"), *GetOwner()->GetName());
	}

	RespawnLocation = RespawnPlayerStart->GetTransform().GetTranslation();
}

bool UFallOffShip::GetPlayerPawnController()
{
	Player = GetWorld()->GetFirstPlayerController();
	if(!Player){ return false; }
	return true;
}

bool UFallOffShip::FindFallZones()
{
	for(AKillZVolume* FallZone : FallZones)
	{
		if(FallZone == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("%s has the FallOffShip script on it, but not enough KillZVolumes set"), *GetOwner()->GetName());
			return false;
		}
	}

	return true;
}

void UFallOffShip::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(DidEnterFallZone(OverlappingActorsOriginalEngine, 0) || DidEnterFallZone(OverlappingActorsOtherEngine, 1))
	{
		// Give some time for the player to realize realize their mistake
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UFallOffShip::RespawnFallenActor, SpawnDelay, false);
	}
}

void UFallOffShip::RespawnFallenActor()
{
	// run destroying of player pawn if an object that is not the player falls off the ship
	if(!(GetOwner()->Tags[0] == "Player")){
		GetOwner()->SetActorLocationAndRotation(OwnerRespawnPoint, ResetRotation, false);
		GetWorld()->GetAuthGameMode()->RestartPlayer(GetWorld()->GetFirstPlayerController());

		APawn* OldPawn = Player->GetPawn();

		OldPawn->DetachFromControllerPendingDestroy();
		OldPawn->Destroy();
	}

	APawn* NewSpawnedPlayer = GetWorld()->SpawnActor<APawn>(PlayerActorToBeSpawned, FTransform(RespawnLocation));
	Player->Possess(NewSpawnedPlayer);
}
 
bool UFallOffShip::DidEnterFallZone(TArray<AActor*> OverlappingActors, int32 FallZone)
{
	if(!FindFallZones()){ return false; }

	FallZones[FallZone]->GetOverlappingActors(OverlappingActors);

	for(AActor* Actor : OverlappingActors)
	{
		if(Actor->Tags[0] == GetOwner()->Tags[0])
		{
			return true;
		}

		if(Actor->Tags[0] == "Player")
		{
			return true;
		}
	}

	return false;
}