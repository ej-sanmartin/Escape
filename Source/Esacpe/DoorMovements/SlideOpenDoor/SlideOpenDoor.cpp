#include "SlideOpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
USlideOpenDoor::USlideOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USlideOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialSlidePosition = GetOwner()->GetActorLocation().Y;
	CurrentSlidePosition = InitialSlidePosition;
	SlideLength = InitialSlidePosition + SlidingDistance;

	FindPressurePlate();
	FindAudioComponent();
}

void USlideOpenDoor::FindPressurePlate()
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate to set"), *GetOwner()->GetName());
	}
}

void USlideOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Audio Component"), *GetOwner()->GetName());
	}

}

// Called every frame
void USlideOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TotalMassOfActors() >= MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void USlideOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentSlidePosition = FMath::Lerp(CurrentSlidePosition, SlideLength, DoorOpenSpeed);
	FVector DoorSlide = GetOwner()->GetActorLocation();
	DoorSlide.Y = CurrentSlidePosition;
	GetOwner()->SetActorLocation(DoorSlide);

	CloseDoorSound = false;
	if(!AudioComponent) { return; }
	if(!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void USlideOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentSlidePosition = FMath::Lerp(CurrentSlidePosition, InitialSlidePosition, DoorCloseSpeed);
	FVector DoorSlide = GetOwner()->GetActorLocation();
	DoorSlide.Y = CurrentSlidePosition;
	GetOwner()->SetActorLocation(DoorSlide);

	OpenDoorSound = false;
	if(!AudioComponent){ return; }
	if(!CloseDoorSound)
	{
		AudioComponent->Play();
	}
	CloseDoorSound = true;
}

float USlideOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;

	if(!PressurePlate){ return TotalMass;}

	PressurePlate->GetOverlappingActors(OverlappingActors);

	for(AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}