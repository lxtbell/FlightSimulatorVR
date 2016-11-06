// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FlightSimulatorVR.h"
#include "FlightSimulatorVRGameMode.h"
#include "FlightSimulatorVRPawn.h"
#include "MainHUD.h"

AFlightSimulatorVRGameMode::AFlightSimulatorVRGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AFlightSimulatorVRPawn::StaticClass();

	HUDClass = AMainHUD::StaticClass();
}
