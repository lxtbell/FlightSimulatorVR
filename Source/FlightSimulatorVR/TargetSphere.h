// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TargetSphere.generated.h"

UCLASS()
class FLIGHTSIMULATORVR_API ATargetSphere : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Sphere, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDestructibleComponent* Sphere;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* ExplosionSound;

public:	
	// Sets default values for this actor's properties
	ATargetSphere();

	virtual void Activate();

	virtual void Explode(const FVector & Location);

private:
	UPROPERTY(Category = Misc, EditAnywhere)
	float SelfDestructionDamage;

	UPROPERTY(Category = Misc, EditAnywhere)
	float SelfDestructionRadius;

	UPROPERTY(Category = Misc, EditAnywhere)
	float SelfDestructionImpulse;

	bool bExploded;

public:
	FORCEINLINE bool IsExploded() const { return bExploded; }
};
