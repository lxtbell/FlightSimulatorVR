// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "Weapon.h"

#include "TargetSphere.h"
#include "Targets.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AWeapon::Activate(class AFlightSimulatorVRPawn* WeaponLauncher)
{
	Launcher = WeaponLauncher;
}

void AWeapon::OnWeaponHit(class AActor* Other, const FVector & HitLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnActorHit %s"), *Other->GetName());

	if (Other->IsA(ATargetSphere::StaticClass()))
	{
		AActor* OtherOwner = Other->GetOwner();
		if (OtherOwner && OtherOwner->IsA(ATargets::StaticClass()))
			Cast<ATargets>(OtherOwner)->OnTargetHit(Other, HitLocation, Launcher, this);
	}
}

void AWeapon::Remove()
{
	Destroy();
}
