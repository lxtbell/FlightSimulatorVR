// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"


UCLASS()
class FLIGHTSIMULATORVR_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMainHUD();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostRender() override;

	enum class Mode
	{
		MainMenu,
		InGame,
		InGameTutorial,
		GameOver
	};

	virtual void SetMode(Mode NewMode);

private:
	Mode CurrentMode;

	class AFlightSimulatorVRPawn* FlightPawn;

	class APilotState* PilotState;
};
