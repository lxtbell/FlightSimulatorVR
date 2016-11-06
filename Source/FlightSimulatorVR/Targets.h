// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Targets.generated.h"


UCLASS()
class FLIGHTSIMULATORVR_API ATargets : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Volume, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Cube;
	
public:	
	// Sets default values for this actor's properties
	ATargets();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnTargetHit(AActor* Target, const FVector & Location, class AFlightSimulatorVRPawn* Pawn);

protected:
	virtual class ATargetSphere* SpawnSphere(const FVector & Location, const FRotator & Rotation);

	virtual void Rebuild();

private:
	UPROPERTY(Category = Volume, EditAnywhere)
	int32 SphereNumber;

	UPROPERTY(Category = Volume, EditAnywhere)
	float RebuildTime;

	struct SphereInfo
	{
		FVector Location;
		FRotator Rotation;
		ATargetSphere* Sphere;

		SphereInfo(const FVector & Location, const FRotator & Rotation, class ATargetSphere* Sphere = nullptr) :
			Location { Location },
			Rotation { Rotation },
			Sphere { Sphere }
		{
		}
	};

	ATargetSphere* SphereTemplate;
	TArray<SphereInfo> Spheres;
	TQueue<SphereInfo*> FracturedSpheres;
};
