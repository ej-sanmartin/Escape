#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "SlideOpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESACPE_API USlideOpenDoor : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	USlideOpenDoor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindPressurePlate();
	void FindAudioComponent();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	float InitialSlidePosition;
	float CurrentSlidePosition;


	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 40.0f;

	float SlideLength = 20.0f;

	UPROPERTY(EditAnywhere)
	float SlidingDistance = 100.0f;

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