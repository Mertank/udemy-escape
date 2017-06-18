// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor() :
	OpenAngle( -70.0f ),
	PressurePlate( nullptr ),
	LastOpenDoor( 0.0f )
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();

	// ...
	ActorThatOpens	= GetWorld()->GetFirstPlayerController()->GetPawn();
	DoorOwner		= GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if ( PressurePlate->IsOverlappingActor( ActorThatOpens ) ) {
		OpenDoor();

		LastOpenDoor = GetWorld()->GetTimeSeconds();
	} else if ( GetWorld()->GetTimeSeconds() > ( LastOpenDoor + DoorCloseDelay ) ) { //Close the door after a certain number of seconds
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor( void ) {
	DoorOwner->SetActorRotation( FRotator( 0.0f, OpenAngle, 0.0f ) );
}

void UOpenDoor::CloseDoor( void ) {
	DoorOwner->SetActorRotation( FRotator( 0.0f, 0.0f, 0.0f ) );
}