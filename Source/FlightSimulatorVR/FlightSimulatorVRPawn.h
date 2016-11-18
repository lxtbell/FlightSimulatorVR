// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"
#include "FlightSimulatorVRPawn.generated.h"


UCLASS(config=Game)
class AFlightSimulatorVRPawn : public APawn
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	//UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UStaticMeshComponent* PlaneMesh;

	UPROPERTY(Category = Plane, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDestructibleComponent* Plane;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneHUD;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MainMenu;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreHUD;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* GameOverText;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* Explosion;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URadialForceComponent* RadialForce;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* ExplosionSound;

	UPROPERTY(Category = Physics, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* StreakSound;

public:
	AFlightSimulatorVRPawn();

	// Begin AActor overrides
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, class AActor * DamageCauser) override;
	// End AActor overrides

	virtual void StartFlying();
	virtual void PlayStreakSound(int32 Streak);

protected:
	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	virtual void StartGame();
	virtual void ExitGame();

	virtual void StartFirePrimary();
	virtual void StopFirePrimary();
	virtual void StartFireSecondary();
	virtual void StopFireSecondary();

	virtual void PitchUpInput(float Val);
	virtual void YawRightInput(float Val);
	virtual void ThrustInput(float Val);
	virtual void RollRightInput(float Val);

	virtual void Fire(class AWeapon* WeaponTemplate, const TArray<FVector> & WeaponLocations, int32 & CurrentWeapon);
	virtual void FirePrimary();
	virtual void FireSecondary();

private:
	/** Max forward speed */
	UPROPERTY(Category = Plane, EditAnywhere)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Plane, EditAnywhere)
	float MinSpeed;

	/** Natural forward speed */
	UPROPERTY(Category = Plane, EditAnywhere)
	float NaturalSpeed;

	UPROPERTY(Category = Plane, EditAnywhere)
	float TargetSpeedChangeSpeed;

	/** How quickly forward speed changes */
	UPROPERTY(Category = Plane, EditAnywhere)
	float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float PitchSpeed;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float YawSpeed;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float RollSpeed;

	UPROPERTY(Category = PrimaryWeapon, EditAnywhere)
	class AWeapon* PrimaryWeaponTemplate;

	UPROPERTY(Category = PrimaryWeapon, EditAnywhere)
	float PrimaryWeaponFireRate;

	UPROPERTY(Category = PrimaryWeapon, EditAnywhere)
	TArray<FVector> PrimaryWeaponLocations;

	UPROPERTY(Category = SecondaryWeapon, EditAnywhere)
	class AWeapon* SecondaryWeaponTemplate;

	UPROPERTY(Category = SecondaryWeapon, EditAnywhere)
	float SecondaryWeaponFireRate;

	UPROPERTY(Category = SecondaryWeapon, EditAnywhere)
	TArray<FVector> SecondaryWeaponLocations;

	UPROPERTY(Category = Misc, EditAnywhere)
	float SelfDestructionDamage;

	UPROPERTY(Category = Misc, EditAnywhere)
	float SelfDestructionRadius;

	UPROPERTY(Category = Misc, EditAnywhere)
	float SelfDestructionImpulse;

	UPROPERTY(Category = Misc, EditAnywhere)
	float ExplodedCameraDistance;

	UPROPERTY(Category = Misc, EditAnywhere)
	float ExplodedCameraSpeed;

	UPROPERTY(Category = Misc, EditAnywhere)
	float ExplodedTimeDilation;

	UPROPERTY(Category = Sound, EditAnywhere)
	TArray<USoundCue*> StreakSounds;

	UPROPERTY(Category = Misc, EditAnywhere)
	FPostProcessSettings ExplodedLensSettings;

	/** Throttle interpolation */
	float CurrentTargetSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	FTimerHandle PrimaryWeaponTimerHandle;
	int32 CurrentPrimaryWeapon;

	FTimerHandle SecondaryWeaponTimerHandle;
	int32 CurrentSecondaryWeapon;

	enum class Stage
	{
		Created,
		Flying,
		Exploded
	};
	Stage CurrentStage;

	class AMainHUD* MainHUD;

	class APilotState* PilotState;

public:
	FORCEINLINE class UStaticMeshComponent* GetMainMenu() const { return MainMenu; }

	FORCEINLINE class UTextRenderComponent* GetScoreHUD() const { return ScoreHUD; }

	FORCEINLINE class UTextRenderComponent* GetGameOverText() const { return GameOverText; }

	FORCEINLINE class APilotState* GetPilotState() const { return PilotState; }

	FORCEINLINE float GetForwardSpeed() const { return CurrentForwardSpeed; }
};
