// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "PilotState.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTSIMULATORVR_API APilotState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(Category = Streak, EditAnywhere)
	float StreakTime;

public:
	APilotState();

	virtual void Tick(float DeltaSeconds) override;

	float Score;

	float SecondsPlayed;

	float TotalWeaponFired;

	float TotalWeaponHit;

	TMap<FName, float> WeaponFired;

	TMap<FName, float> WeaponHit;

	int32 CurrentStreak;

	float CurrentStreakTime;

	void RecordWeaponFire(const FName & WeaponType);

	void RecordWeaponHit(const FName & WeaponType);

	virtual float GetScorePerMinute();

	virtual float GetAccuracy();
};
