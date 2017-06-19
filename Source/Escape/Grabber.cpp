// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber() :
	GrabDistance( 0.0f ),
	m_physicsHandle( nullptr ),
	m_inputComponent( nullptr )
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

	//Find attached physics component
	m_physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if ( m_physicsHandle == nullptr ) {
		UE_LOG( LogTemp, Error, TEXT( "%s does not have a physics handle attached to it" ), *GetOwner()->GetName() );
	}

	//Find attached input component
	m_inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if ( m_inputComponent != nullptr ) {
		m_inputComponent->BindAction( TEXT( "Grab" ), IE_Pressed, this, &UGrabber::Grab );
		m_inputComponent->BindAction( TEXT( "Grab" ), IE_Released, this, &UGrabber::Release );
	} else {
		UE_LOG( LogTemp, Error, TEXT( "%s does not have an input component attached to it" ), *GetOwner()->GetName() );
	}
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if ( m_physicsHandle->GrabbedComponent ) {
		m_physicsHandle->SetTargetLocation(
			GetReachLocation()
		);
	}
}

AActor* UGrabber::GetFirstActorInReach( void ) {
	//UE_LOG( LogTemp, Warning, TEXT( "Player location: %s and rotation: %s" ), *playerLocation.ToString(), *playerRotation.ToString() );

	///Draw grab distance
	//DrawDebugLine( GetWorld(), playerLocation, playerLocation + ( playerRotation.Vector() * GrabDistance ), FColor::Red, false, -1.0f, 0, 4.0f );

	///Line trace reach distance
	FHitResult				lineTraceResult;
	FCollisionQueryParams	traceParams( FName( TEXT( "" ) ), false, GetOwner() );

	GetWorld()->LineTraceSingleByObjectType(
		lineTraceResult,
		GetOwner()->GetActorLocation(), GetReachLocation(),
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		traceParams
	);

	if ( lineTraceResult.Actor.IsValid() ) {
		UE_LOG( LogTemp, Warning, TEXT( "Hit actor: %s" ), *lineTraceResult.Actor->GetName() );		
		return lineTraceResult.Actor.Get();
	}

	return nullptr;
}

void UGrabber::Grab( void ) {
	AActor* actorToGrab = GetFirstActorInReach();

	if ( actorToGrab != nullptr ) {
		m_physicsHandle->GrabComponentAtLocationWithRotation(
			actorToGrab->FindComponentByClass<UPrimitiveComponent>(),
			NAME_None,
			actorToGrab->GetActorLocation(),
			actorToGrab->GetActorRotation()
		);
	}
}

void UGrabber::Release( void ) {
	m_physicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLocation( void ) {
	///Grab player location/rotation
	FVector		playerLocation;
	FRotator	playerRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( playerLocation, playerRotation );

	return playerLocation + ( playerRotation.Vector() * GrabDistance );
}