// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "Targets.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "Public/EngineUtils.h"
#include "Components/ActorComponent.h"
#include "Components/DestructibleComponent.h"
#include "TargetSphere.h"
#include "PilotState.h"
#include "FlightSimulatorVRPawn.h"
#include "ResourceGuy.h"
#include "Weapon.h"


// Sets default values
ATargets::ATargets()
{
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube0"));
	Cube->bHiddenInGame = true;
	Cube->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = Cube;

	SphereTemplate = nullptr;
	SphereNumber = 20;
	RebuildTime = 5.f;
}

// Called when the game starts or when spawned
void ATargets::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 SphereId = 0; SphereId < SphereNumber; ++SphereId)
	{
		FVector CubeSize = GetActorScale() * 50;
		FVector Location(FMath::RandRange(0.f, CubeSize.X), FMath::RandRange(0.f, CubeSize.Y), FMath::RandRange(0.f, CubeSize.Z));
		Location = Location * 2 - CubeSize;
		Location = GetActorLocation() + GetActorRotation().RotateVector(Location);

		FRotator Rotation(FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f));

		//if (SphereTemplate != nullptr)
		//	UE_LOG(LogTemp, Warning, TEXT("ATargets::BeginPlay %s %s"), *Location.ToString(), *SphereTemplate->GetName());

		Spheres.Add({ Location, Rotation, SpawnSphere(Location, Rotation) });
	}
}

void ATargets::OnTargetHit(AActor* Target, const FVector & Location, class AFlightSimulatorVRPawn* Pawn, class AWeapon* Weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("ATargets::OnTargetHit %s"), *Target->GetName());

	for (SphereInfo & Info : Spheres)
		if (Info.Sphere == Target && !Info.Sphere->IsExploded())
		{
			Info.Sphere->Explode(Location);

			FracturedSpheres.Enqueue(&Info);
			FTimerHandle DestroyTimerHandle;
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ATargets::Rebuild, RebuildTime, false);

			APilotState* PilotState = Pawn->GetPilotState();
			if (PilotState != nullptr)
			{
				PilotState->RecordWeaponHit(Weapon->GetWeaponType());

				Pawn->PlayStreakSound(PilotState->CurrentStreak);
			}

			break;
		}
}

class ATargetSphere* ATargets::SpawnSphere(const FVector & Location, const FRotator & Rotation)
{
	FActorSpawnParameters Parameters;
	if (SphereTemplate != nullptr)
		Parameters.Template = SphereTemplate;
	Parameters.Owner = this;

	ATargetSphere* NewActor = GetWorld()->SpawnActor<ATargetSphere>(ATargetSphere::StaticClass(), Location, Rotation, Parameters);
	NewActor->Activate();
	
	return NewActor;
}

void ATargets::Rebuild()
{
	SphereInfo* Info;
	FracturedSpheres.Dequeue(Info);

	AActor* Target = Info->Sphere;

	Info->Sphere = SpawnSphere(Info->Location, Info->Rotation);
	UE_LOG(LogTemp, Warning, TEXT("ATargets::Rebuild %s"), *Target->GetName());

	Target->Destroy();
}
