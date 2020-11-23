#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "GameFramework/KillZVolume.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Math/Rotator.h"
#include "UObject/SoftObjectPtr.h"
#include "FallOffShip.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESACPE_API UFallOffShip : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	UFallOffShip();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DebugPlayerControllerSearchVerification();
	void SetupFallZones();
	void SetupSpawnLocations();
	bool DidEnterFallZone(TArray<AActor*> OverlappingActors, int32 FallZone);
	bool GetPlayerPawnController();
	bool FindFallZones();
	void RespawnFallenActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<AKillZVolume> OriginFallZone = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<AKillZVolume> OtherFallZone = nullptr;


private:
	// hard coded init the 2 falling zone instead of making it dynamic since this only has 2
	TArray<AKillZVolume*> FallZones = { nullptr, nullptr };

	FRotator ResetRotation = {0.0f, 0.0f, 0.0f};
	FVector DefaultRespawnPoint = { -9999.9f, -9999.9f, -9999.9f }; // used to calculate error

	TArray<AActor*> OverlappingActorsOriginalEngine;
	TArray<AActor*> OverlappingActorsOtherEngine;

	APlayerController* Player;

	UPROPERTY(EditAnywhere, Category = Spawn)
	float SpawnDelay = 2.0f;

	// Respawn vector for item attached to this script
	UPROPERTY(EditAnywhere, Category = Spawn)
	FVector OwnerRespawnPoint = DefaultRespawnPoint;

	// Respawn vector for the player and player pawn, please attach player start
	UPROPERTY(EditAnywhere, Category = Spawn)
	TSoftObjectPtr<APlayerStart> PlayerStartSpawnPoint;

	APlayerStart* RespawnPlayerStart;

	FVector RespawnLocation;

	// Pawn to be spawned
	UPROPERTY(EditAnywhere, Category = Spawn)
	UBlueprintGeneratedClass* PlayerActorToBeSpawned;
};