// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FLIGHTSIMULATORVR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Activate(class AFlightSimulatorVRPawn* WeaponLauncher);

protected:
	virtual void OnWeaponHit(class AActor* Other, const FVector & HitLocation);

	virtual void Remove();

	class AFlightSimulatorVRPawn* Launcher;

public:
	FORCEINLINE virtual FName GetWeaponType() const { return TEXT("UnknownWeapon"); }

	FORCEINLINE virtual class UClass* GetWeaponClass() const { return StaticClass(); }

	FORCEINLINE class AFlightSimulatorVRPawn* GetLauncher() const { return Launcher; }
};
