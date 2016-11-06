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

	float MissileFired;

	float MissileHit;

	int32 CurrentStreak;

	float CurrentStreakTime;
};
