// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FlightSimulatorVR.h"
#include "FlightSimulatorVRPawn.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "Components/DestructibleComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/AudioComponent.h"
#include "Public/EngineUtils.h"
#include "Missile.h"
#include "Targets.h"
#include "TargetSphere.h"
#include "PilotState.h"
#include "MainHUD.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Scene.h"


AFlightSimulatorVRPawn::AFlightSimulatorVRPawn()
{
	// Create static mesh component
	//PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	//RootComponent = PlaneMesh;

	Plane = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Plane0"));
	//Plane->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = Plane;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 40.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bDoCollisionTest = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	Explosion = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion0"));
	Explosion->SetupAttachment(Plane);
	Explosion->bAutoActivate = false;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce0"));
	RadialForce->SetupAttachment(Explosion);
	RadialForce->bAutoActivate = false;
	RadialForce->Radius = 500.f;
	RadialForce->DestructibleDamage = 4.f;

	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound0"));
	ExplosionSound->SetupAttachment(Explosion);
	ExplosionSound->bAutoActivate = false;

	StreakSound = CreateDefaultSubobject<UAudioComponent>(TEXT("StreakSound0"));
	StreakSound->SetupAttachment(Plane);
	StreakSound->bAutoActivate = false;

	// Set handling parameters
	MaxSpeed = 4000.f;
	MinSpeed = 250.f;
	NaturalSpeed = 1000.f;
	Acceleration = 1.f;
	PitchSpeed = 90.f;
	YawSpeed = 15.f;
	RollSpeed = 90.f;

	FireRate = 0.5f;
	MissileLocations.Add(FVector(0));
	MissileLocations.Add(FVector(0));

	ExplodedCameraDistance = 800.f;
	ExplodedCameraSpeed = 10.f;
	ExplodedTimeDilation = 0.2f;
	SelfDestructionDamage = 25000.f;
	SelfDestructionRadius = 200.f;
	SelfDestructionImpulse = 0.f;

	ExplodedLenseSetting = FPostProcessSettings();

	CurrentStage = Stage::Created;

	bCanBeDamaged = false;
	Plane->SetVisibility(false);
}


void AFlightSimulatorVRPawn::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AFlightSimulatorVRPawn::BeginPlay"));

	ThrottleA = NaturalSpeed;
	ThrottleB = (MaxSpeed - MinSpeed) / 2;
	ThrottleC = (MaxSpeed + MinSpeed) / 2 - NaturalSpeed;
	CurrentForwardSpeed = NaturalSpeed;
	CurrentPitchSpeed = 0.f;
	CurrentYawSpeed = 0.f;
	CurrentRollSpeed = 0.f;

	MissileTemplate = nullptr;
	for (TActorIterator<AMissile> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		UE_LOG(LogTemp, Warning, TEXT("AFlightSimulatorVRPawn::ActorItr %s =?= %s"), *ActorItr->GetActorLabel(), *(GetActorLabel() + "_MissileTemplate"));
		if (ActorItr->GetActorLabel() == GetActorLabel() + "_MissileTemplate")
		{
			UE_LOG(LogTemp, Warning, TEXT("AFlightSimulatorVRPawn::ActorItr Found!"));
			MissileTemplate = *ActorItr;
		}
	}
	TotalMissiles = MissileLocations.Num();
	CurrentMissile = 0;

	PilotState = nullptr;
	MainHUD = nullptr;
	if (GetController()->IsA(APlayerController::StaticClass()))
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		
		AHUD* HUD = PlayerController->GetHUD();
		if (HUD->IsA(AMainHUD::StaticClass()))
		{
			MainHUD = Cast<AMainHUD>(HUD);
			MainHUD->SetMode(AMainHUD::Mode::InGame);
		}

		APlayerState* PlayerState = PlayerController->PlayerState;
		if (PlayerState->IsA(APilotState::StaticClass()))
		{
			PilotState = Cast<APilotState>(PlayerState);
		}
	}
}

void AFlightSimulatorVRPawn::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	//if (FMath::Rand() % 100 == 0)
	//	UE_LOG(LogTemp, Warning, TEXT("AFlightSimulatorVRPawn::Tick [CurrentForwardSpeed = %.4f] [bExploded = %d] [GetActorLocation = %s] [GetActorRotation = %s]"), CurrentForwardSpeed, bExploded ? 1 : 0, *GetActorLocation().ToString(), *GetActorRotation().ToString());

	if (CurrentStage == Stage::Flying)
	{
		const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

		// Move plan forwards (with sweep so we stop when we collide with things)
		AddActorLocalOffset(LocalMove, true);

		// Calculate change in rotation this frame
		FRotator DeltaRotation(0, 0, 0);
		DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
		DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
		DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

		// Rotate plane
		AddActorLocalRotation(FQuat(DeltaRotation));
	}
	else if (CurrentStage == Stage::Exploded)
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ExplodedCameraDistance, GetWorld()->GetDeltaSeconds(), ExplodedCameraSpeed);
	}
}

void AFlightSimulatorVRPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	UE_LOG(LogTemp, Warning, TEXT("AFlightSimulatorVRPawn::NotifyHit %s"), *Other->GetName());

	if (CurrentStage == Stage::Flying)
	{
		bool bExplode = true;

		if (Other->IsA(AMissile::StaticClass()))
			if (Cast<AMissile>(Other)->GetLauncher() == this)
				bExplode = false;

		if (Other->IsA(ATargetSphere::StaticClass()))
		{
			bExplode = false;

			// Deflect along the surface when we collide.
			FRotator CurrentRotation = GetActorRotation();
			SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
		}
			
		if (bExplode)
		{
			bCanBeDamaged = true;
			Plane->ApplyRadiusDamage(SelfDestructionDamage, HitLocation, SelfDestructionRadius, SelfDestructionImpulse, true);

			Explosion->Activate();
			RadialForce->FireImpulse();
			ExplosionSound->Play();
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), ExplodedTimeDilation);
			
			Camera->PostProcessSettings = ExplodedLenseSetting;

			CurrentStage = Stage::Exploded;

			AController* Controller = GetController();
			if (Controller->IsA(APlayerController::StaticClass()))
			{
				AHUD* HUD = Cast<APlayerController>(Controller)->GetHUD();
				if (HUD->IsA(AMainHUD::StaticClass()))
					Cast<AMainHUD>(HUD)->SetMode(AMainHUD::Mode::GameOver);
			}
		}
	}
}

float AFlightSimulatorVRPawn::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, class AActor * DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("AFlightSimulatorVRPawn::TakeDamage %.4f %s"), ActualDamage, *DamageCauser->GetName());

	return ActualDamage;
}

void AFlightSimulatorVRPawn::StartFlying()
{
	if (CurrentStage != Stage::Created)
		return;

	CurrentStage = Stage::Flying;
	
	Plane->SetVisibility(true);
}

void AFlightSimulatorVRPawn::PlayStreakSound(int32 Streak)
{
	Streak = FMath::Min(Streak, StreakSounds.Num());

	if (Streak <= 0)
		return;

	StreakSound->Stop();
	StreakSound->Sound = StreakSounds[Streak - 1];
	StreakSound->Play();
}

void AFlightSimulatorVRPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFlightSimulatorVRPawn::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFlightSimulatorVRPawn::StopFire);
	PlayerInputComponent->BindAxis("PitchUp", this, &AFlightSimulatorVRPawn::PitchUpInput);
	PlayerInputComponent->BindAxis("YawRight", this, &AFlightSimulatorVRPawn::YawRightInput);
	PlayerInputComponent->BindAxis("Thrust", this, &AFlightSimulatorVRPawn::ThrustInput);
	PlayerInputComponent->BindAxis("RollRight", this, &AFlightSimulatorVRPawn::RollRightInput);
}

void AFlightSimulatorVRPawn::StartFire()
{
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AFlightSimulatorVRPawn::Fire, FireRate, true, 0.f);
}

void AFlightSimulatorVRPawn::StopFire()
{
	if (!FireTimerHandle.IsValid())
		return;
	
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AFlightSimulatorVRPawn::PitchUpInput(float Val)
{
	if (CurrentStage != Stage::Flying)
		return;

	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * PitchSpeed);

	// When steering, we decrease pitch slightly
	//TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AFlightSimulatorVRPawn::YawRightInput(float Val)
{
	if (CurrentStage != Stage::Flying)
		return;

	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * YawSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	//const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	//float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	//CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AFlightSimulatorVRPawn::ThrustInput(float Val)
{
	if (CurrentStage != Stage::Flying)
		return;

	// Is there no input?
	//bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);

	// Calculate new speed
	float JetSpeed = ThrottleA + ThrottleB * Val + ThrottleC * Val * Val;
	float CurrentAcc = Acceleration * (JetSpeed - CurrentForwardSpeed);
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);

	// Clamp between MinSpeed and MaxSpeed
	//CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
	CurrentForwardSpeed = NewForwardSpeed;
}

void AFlightSimulatorVRPawn::RollRightInput(float Val)
{
	if (CurrentStage != Stage::Flying)
		return;

	// Target roll speed is based on input
	float TargetRollSpeed = (Val * RollSpeed);

	// Smoothly interpolate to target roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AFlightSimulatorVRPawn::Fire()
{
	if (CurrentStage != Stage::Flying)
		return;

	FRotator ActorRotation = GetActorRotation();

	// Make a location for the new actor to spawn at
	FVector NewLocation = GetActorLocation() + ActorRotation.RotateVector(MissileLocations[CurrentMissile]);
	CurrentMissile = (CurrentMissile + 1) % TotalMissiles;

	// Spawn the new actor (Using GetClass() instead of AMySpawner so that if someone derives a new class  
	// from AMySpawner we spawn an instance of that class instead)
	FActorSpawnParameters Parameters;
	if (MissileTemplate != nullptr)
		Parameters.Template = MissileTemplate;

	AMissile* NewActor = GetWorld()->SpawnActor<AMissile>(AMissile::StaticClass(), NewLocation, ActorRotation, Parameters);
	NewActor->Activate(CurrentForwardSpeed, this);

	if (PilotState != nullptr)
		PilotState->MissileFired += 1;
}
