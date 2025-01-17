// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *Owner->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume 
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO Make everything event driven (doesn't need to be called every tick)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TSet<AActor*> OverlappingActors;
	
	if (!PressurePlate) { return TotalMass; }

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (const auto* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *OverlappingActor->GetName());
			/// "PrimitiveComponents are SceneComponents that contain or generate some sort of geometry, generally to be rendered or used as collision data."
			TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
		
	}

	return TotalMass;
}

