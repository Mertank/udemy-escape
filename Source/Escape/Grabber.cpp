// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber() :
	GrabDistance( 0.0f ),
	m_physicsHandle( nullptr )
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	UE_LOG( LogTemp, Warning, TEXT( "Grabber reporting for duty" ) );

	m_physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if ( m_physicsHandle == nullptr ) {
		UE_LOG( LogTemp, Error, TEXT( "%s does not have a physics handle attached to it" ), *GetOwner()->GetName() );
	}
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	///Grab player location/rotation
	FVector		playerLocation;
	FRotator	playerRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( playerLocation, playerRotation );
	
	//UE_LOG( LogTemp, Warning, TEXT( "Player location: %s and rotation: %s" ), *playerLocation.ToString(), *playerRotation.ToString() );

	///Draw grab distance
	//DrawDebugLine( GetWorld(), playerLocation, playerLocation + ( playerRotation.Vector() * GrabDistance ), FColor::Red, false, -1.0f, 0, 4.0f );

	///Line trace reach distance
	FHitResult				lineTraceResult;
	FCollisionQueryParams	traceParams( FName( TEXT( "" ) ), false, GetOwner() );
	
	GetWorld()->LineTraceSingleByObjectType(
		lineTraceResult,
		playerLocation, playerLocation + ( playerRotation.Vector() * GrabDistance ),
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		traceParams
	);

	if ( lineTraceResult.Actor.IsValid() ) {
		UE_LOG( LogTemp, Warning, TEXT( "Hit actor: %s" ), *lineTraceResult.Actor->GetName() );
	}
}

