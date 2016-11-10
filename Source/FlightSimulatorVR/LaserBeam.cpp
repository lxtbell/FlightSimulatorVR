// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "LaserBeam.h"

#include "TargetSphere.h"
#include "Targets.h"
#include "FlightSimulatorVRPawn.h"


// Sets default values
ALaserBeam::ALaserBeam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Laser = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Laser0"));
	Laser->bAutoActivate = false;
	RootComponent = Laser;

	LaserActiveLength = 100.f;
	LaserLength = 100000.f;
	LaserLifeSpan = 1.f;
}

// Called when the game starts or when spawned
void ALaserBeam::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserBeam::Activate(class AFlightSimulatorVRPawn* WeaponLauncher)
{
	Super::Activate(WeaponLauncher);

	AttachToActor(WeaponLauncher, { EAttachmentRule::KeepWorld, true });

	Laser->Activate();

	FVector Origin = GetActorLocation(); FRotator Rotation = GetActorRotation();
	FVector StartLocation = Origin + Rotation.RotateVector(FVector(LaserActiveLength, 0, 0));
	FVector EndLocation = Origin + Rotation.RotateVector(FVector(LaserLength, 0, 0));

	FHitResult HitResult; FCollisionObjectQueryParams CollisionObjectQueryParams;
	GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, CollisionObjectQueryParams);
	if (HitResult.Actor.IsValid())
	{
		OnWeaponHit(HitResult.Actor.Get(), HitResult.ImpactPoint);
	}

	FTimerHandle RemoveTimerHandle;
	GetWorldTimerManager().SetTimer(RemoveTimerHandle, this, &ALaserBeam::Remove, LaserLifeSpan, false);
}

// Called every frame
void ALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
