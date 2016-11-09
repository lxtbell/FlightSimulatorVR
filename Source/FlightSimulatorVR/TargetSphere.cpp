// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "TargetSphere.h"
#include "Components/DestructibleComponent.h"


// Sets default values
ATargetSphere::ATargetSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Sphere0"));
	//Sphere->SetVisibility(false);
	//Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = Sphere;

	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound0"));
	ExplosionSound->SetupAttachment(Sphere);
	ExplosionSound->bAutoActivate = false;

	SelfDestructionDamage = 25000.f;
	SelfDestructionRadius = 5000.f;
	SelfDestructionImpulse = 0.f;

	bExploded = false;
}

void ATargetSphere::Activate()
{
	//Sphere->SetVisibility(true);
	//Sphere->SetCollisionProfileName(TEXT("Destructible"));
}

void ATargetSphere::Explode(const FVector & Location)
{
	bExploded = true;
	ExplosionSound->Play();
	Sphere->ApplyRadiusDamage(SelfDestructionDamage, Location, SelfDestructionRadius, SelfDestructionImpulse, true);
}
