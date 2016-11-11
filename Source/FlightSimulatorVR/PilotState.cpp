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

void APilotState::RecordWeaponFire(const FName & WeaponType)
{
	if (!WeaponFired.Contains(WeaponType))
		WeaponFired.Add(WeaponType, 0);

	TotalWeaponFired += 1;
	WeaponFired[WeaponType] += 1;
}

void APilotState::RecordWeaponHit(const FName & WeaponType)
{
	if (!WeaponHit.Contains(WeaponType))
		WeaponHit.Add(WeaponType, 0);

	Score += 1;
	TotalWeaponHit += 1;
	WeaponHit[WeaponType] += 1;

	CurrentStreak += 1;
	CurrentStreakTime = 0;
}

float APilotState::GetScorePerMinute()
{
	return (SecondsPlayed == 0) ? 0 : (Score / SecondsPlayed * 60);
}

float APilotState::GetAccuracy()
{
	return (TotalWeaponFired == 0) ? 1 : (TotalWeaponHit / TotalWeaponFired);
}
