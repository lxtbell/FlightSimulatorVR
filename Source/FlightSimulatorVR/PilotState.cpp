// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "PilotState.h"


APilotState::APilotState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StreakTime = 4.f;

	Score = 0.f;
	SecondsPlayed = 0.f;
	MissileFired = 0.f;
	MissileHit = 0.f;
	CurrentStreak = 0;
	CurrentStreakTime = 0.f;
}

void APilotState::Tick(float DeltaSeconds)
{
	SecondsPlayed += DeltaSeconds;
	CurrentStreakTime += DeltaSeconds;

	if (CurrentStreakTime >= StreakTime)
	{
		CurrentStreak = 0;
		CurrentStreakTime = 0.f;
	}
}
