// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "MainHUD.h"
#include "Public/EngineUtils.h"
#include "FlightSimulatorVRPawn.h"
#include "Engine/Canvas.h"
#include "PilotState.h"
#include "ResourceGuy.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "MainHUD"


AMainHUD::AMainHUD()
{
}

// Called when the game starts or when spawned
void AMainHUD::BeginPlay()
{
	Super::BeginPlay();
	
	FlightPawn = nullptr;
	PilotState = nullptr;
	if (PlayerOwner)
	{
		APawn* Pawn = PlayerOwner->GetPawn();
		if (Pawn && Pawn->IsA(AFlightSimulatorVRPawn::StaticClass()))
			FlightPawn = Cast<AFlightSimulatorVRPawn>(Pawn);

		APlayerState* PlayerState = PlayerOwner->PlayerState;
		if (PlayerState && PlayerState->IsA(APilotState::StaticClass()))
			PilotState = Cast<APilotState>(PlayerState);
	}

	SetMode(Mode::MainMenu);
}

void AMainHUD::PostRender()
{
	Super::PostRender();

	switch (CurrentMode)
	{
	case Mode::MainMenu:
		break;
	case Mode::InGameTutorial:
	case Mode::InGame:
		if (FlightPawn != nullptr && PilotState != nullptr)
		{
			FNumberFormattingOptions Options;
			Options.SetMaximumFractionalDigits(0);

			FlightPawn->GetScoreText()->SetText(FText::Format(LOCTEXT("Score", "Score {0}    Score Per Minute {1}\nAccuracy {2}    Current Streak {3}"),
				FText::AsNumber(PilotState->Score, &Options), FText::AsNumber(PilotState->GetScorePerMinute(), &Options), FText::AsPercent(PilotState->GetAccuracy(), &Options), FText::AsNumber(PilotState->CurrentStreak)));
		}
		break;
	case Mode::GameOver:
		if (FlightPawn != nullptr)
		{
			FlightPawn->GetGameOverText()->SetText(LOCTEXT("Wasted", "Wasted"));
		}
		break;
	}
}

void AMainHUD::SetMode(Mode NewMode)
{
	CurrentMode = NewMode;

	if (!PlayerOwner || FlightPawn == nullptr)
		return;

	FlightPawn->GetMainMenu()->SetVisibility(false);
	FlightPawn->GetScoreText()->SetVisibility(false);
	FlightPawn->GetGameOverText()->SetVisibility(false);

	switch (CurrentMode)
	{
	case Mode::MainMenu:
		FlightPawn->GetMainMenu()->SetVisibility(true);

		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->bEnableMouseOverEvents = true;
		PlayerOwner->bEnableClickEvents = true;
		break;
	case Mode::GameOver:
		FlightPawn->GetGameOverText()->SetVisibility(true);

		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->bEnableMouseOverEvents = true;
		PlayerOwner->bEnableClickEvents = true;
		break;
	case Mode::InGame:
	case Mode::InGameTutorial:
		FlightPawn->GetScoreText()->SetVisibility(true);
		FlightPawn->StartFlying();

		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->bEnableMouseOverEvents = false;
		PlayerOwner->bEnableClickEvents = false;
		break;
	}
}

#undef LOCTEXT_NAMESPACE
