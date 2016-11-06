// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Missile.generated.h"

UCLASS()
class FLIGHTSIMULATORVR_API AMissile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Plane, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Missile;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* Trail;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* MissileSound;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* Explosion;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URadialForceComponent* RadialForce;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* ExplosionSound;

public:	
	// Sets default values for this actor's properties
	AMissile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Activate(float LaunchSpeed = 0, class AActor* MissileLauncher = nullptr, class APilotState* LauncherPilotState = nullptr);

	virtual void Unlock();
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void Explode();

protected:
	virtual void Remove();

private:
	UPROPERTY(Category = Missile, EditAnywhere)
	float LockTime;

	UPROPERTY(Category = Missile, EditAnywhere)
	float FlyTime;

	UPROPERTY(Category = Missile, EditAnywhere)
	float BurnTime;

	UPROPERTY(Category = Missile, EditAnywhere)
	float InitialSpeed;

	UPROPERTY(Category = Missile, EditAnywhere)
	float MaxSpeed;

	UPROPERTY(Category = Missile, EditAnywhere)
	float Acceleration;

	float CurrentForwardSpeed;

	enum class Stage
	{
		Created,
		Activated,
		Unlocked,
		Exploded,
		Removed
	};
	Stage CurrentStage;

	class AActor* Launcher;

	class APilotState* PilotState;

public:
	FORCEINLINE class AActor* GetLauncher() const { return Launcher; }
};
