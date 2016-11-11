// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "LaserBeam.generated.h"


UCLASS()
class FLIGHTSIMULATORVR_API ALaserBeam : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(Category = Laser, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* Laser;

public:
	// Sets default values for this actor's properties
	ALaserBeam();

	virtual void Activate(class AFlightSimulatorVRPawn* WeaponLauncher) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(Category = Laser, EditAnywhere)
	float LaserActiveLength;

	UPROPERTY(Category = Laser, EditAnywhere)
	float LaserLength;

	UPROPERTY(Category = Laser, EditAnywhere)
	float LaserLifeSpan;

public:
	FORCEINLINE virtual class UClass* GetWeaponClass() const { return StaticClass(); }

	FORCEINLINE virtual FName GetWeaponType() const { return TEXT("Laser"); }
};
