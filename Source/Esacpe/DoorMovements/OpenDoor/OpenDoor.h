#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESACPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindPressurePlate();
	void FindAudioComponent();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 40.0f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.0f;

	float DoorLastOpened = 0.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.8f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 0.04f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 0.1f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent = nullptr;
};
