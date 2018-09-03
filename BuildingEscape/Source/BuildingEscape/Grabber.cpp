// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// PhysicsHandle found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle component of %s is missing!"), *GetOwner()->GetName())
	}
}
/// Look for attached Input Components (only appears at run time)
void UGrabber::SetupInputComponent()
{
	/// InputComponent = GetOwner()->InputComponent; works too?
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn input component of %s found!"), *GetOwner()->GetName())
			/// Bind the input action
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn input component of %s is missing!"), *GetOwner()->GetName())
	}
}



void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();

	/// If we hit something then attach a physics handle
	// TODO attach physics handle

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
	// TODO release physics handle
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
		// move the object that we're holding

	/// See what we hit

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// The ue_log macro wants a pointer to a tchar
	/*UE_LOG(LogTemp, Warning, TEXT("Player view point: %s, Player view point rotation: %s"),
	*PlayerViewPointLocation.ToString(),
	*PlayerViewPointRotation.ToString()
	)*/

	// For testing purpose
	//FVector LineTraceEnd = PlayerViewPointLocation + FVector(0.f, 0.f, 50.f);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;


	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (ALA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), /// ECollisionChannel is an enum
		TraceParameters
	);

	// Log the name of the Actor to the console
	AActor * ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s got hit by the Line-trace"), *(ActorHit->GetName()))
	}


	return FHitResult();
}

