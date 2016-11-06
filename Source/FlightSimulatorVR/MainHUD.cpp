// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "MainHUD.h"
#include "Public/EngineUtils.h"
#include "FlightSimulatorVRPawn.h"
#include "Engine/Canvas.h"
#include "PilotState.h"


AMainHUD::AMainHUD()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture> MenuBackground;
		ConstructorHelpers::FObjectFinderOptional<UFont> MenuFont;
		ConstructorHelpers::FObjectFinderOptional<UFont> WastedFont;
		FConstructorStatics() :
			MenuBackground(TEXT("/Game/Interface/New_York_Ruins.New_York_Ruins")),
			MenuFont(TEXT("/Game/Interface/OLDENGL.OLDENGL")),
			WastedFont(TEXT("/Game/Interface/pricedown_bl.pricedown_bl"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	MenuBackground = ConstructorStatics.MenuBackground.Get();
	MenuFont = ConstructorStatics.MenuFont.Get();
	WastedFont = ConstructorStatics.WastedFont.Get();

	Buttons.Add("Start Game", (new UButtonStartGame())->Set("Start Game", WhiteColor, RedColor));
	Buttons.Add("Start With Tutorial", (new UButtonStartWithTutorial())->Set("Start With Tutorial", WhiteColor, RedColor));
	Buttons.Add("Exit Game", (new UButtonExitGame())->Set("Exit Game", WhiteColor, RedColor));
}

// Called when the game starts or when spawned
void AMainHUD::BeginPlay()
{
	Super::BeginPlay();
	
	SetMode(Mode::MainMenu);

	APlayerState* PlayerState = GetOwningPlayerController()->PlayerState;
	if (PlayerState->IsA(APilotState::StaticClass()))
		PilotState = Cast<APilotState>(PlayerState);
	else
		PilotState = nullptr;
}

void AMainHUD::PostRender()
{
	Super::PostRender();

	float ScreenX = Canvas->SizeX, ScreenY = Canvas->SizeY;

	float FontSize = FMath::Min(ScreenX / 1920, ScreenY / 1080);
	float FontScale = FontSize / MenuFont->LegacyFontSize;

	//if (FMath::Rand() % 100 == 0)
	//	UE_LOG(LogTemp, Warning, TEXT("AMainHUD::PostRender %.4f"), FontSize);

	switch (CurrentMode)
	{
	case Mode::MainMenu:
		DrawTexture(MenuBackground, 0, 0, ScreenX, ScreenY, 0, 0, 1, 1);
		DrawText(TEXT("Flight Simulator VR"), WhiteColor, ScreenX * 0.3, ScreenY * 0.1, MenuFont, FontScale * 120);

		Buttons[TEXT("Start Game")]->Set(ScreenX * 0.1, ScreenY * 0.6, MenuFont, FontScale * 60, FVector2D(6.6, 1.5) * FontSize * 60)->Draw(this);
		Buttons[TEXT("Start With Tutorial")]->Set(ScreenX * 0.1, ScreenY * 0.7, MenuFont, FontScale * 60, FVector2D(11, 1.5) * FontSize * 60)->Draw(this);
		Buttons[TEXT("Exit Game")]->Set(ScreenX * 0.1, ScreenY * 0.8, MenuFont, FontScale * 60, FVector2D(6, 1.5) * FontSize * 60)->Draw(this);

		//RenderHitBoxes(Canvas->Canvas);
		break;
	case Mode::InGameTutorial:
	case Mode::InGame:
		if (PilotState != nullptr)
			DrawText(FString::Printf(TEXT("Score %.0f\tScore Per Minute %.0f\tAccuracy %.2f\tCurrentStreak %d"), 
				PilotState->Score, PilotState->Score / PilotState->SecondsPlayed * 60, PilotState->MissileHit / PilotState->MissileFired, PilotState->CurrentStreak), 
				WhiteColor, ScreenX * 0.2, ScreenY * 0.9, MenuFont, FontScale * 40);
		break;
	case Mode::GameOver:
		DrawText(TEXT("Wasted"), RedColor, ScreenX * 0.4, ScreenY * 0.4, WastedFont, FontScale * 110);
		Buttons[TEXT("Exit Game")]->Set(ScreenX * 0.9, ScreenY * 0.95, MenuFont, FontScale * 40, FVector2D(6, 1.5) * FontSize * 60)->Draw(this);
		break;
	}
}

void AMainHUD::SetMode(Mode NewMode)
{
	CurrentMode = NewMode;

	switch (CurrentMode)
	{
	case Mode::MainMenu:
	case Mode::GameOver:
		GetOwningPlayerController()->bShowMouseCursor = true;
		GetOwningPlayerController()->bEnableMouseOverEvents = true;
		GetOwningPlayerController()->bEnableClickEvents = true;
		break;
	case Mode::InGame:
	case Mode::InGameTutorial:
		GetOwningPlayerController()->bShowMouseCursor = false;
		GetOwningPlayerController()->bEnableMouseOverEvents = false;
		GetOwningPlayerController()->bEnableClickEvents = false;

		APawn* Pawn = GetOwningPlayerController()->GetPawn();
		if (Pawn->IsA(AFlightSimulatorVRPawn::StaticClass()))
			Cast<AFlightSimulatorVRPawn>(Pawn)->StartFlying();
		break;
	}
}

void AMainHUD::UButtonStartGame::NotifyHitBoxClick(class AMainHUD* HUD)
{
	HUD->SetMode(Mode::InGame);
}

void AMainHUD::UButtonStartWithTutorial::NotifyHitBoxClick(class AMainHUD* HUD)
{
	HUD->SetMode(Mode::InGameTutorial);
}

void AMainHUD::UButtonExitGame::NotifyHitBoxClick(class AMainHUD* HUD)
{
	HUD->GetOwningPlayerController()->ConsoleCommand("quit");
}

void AMainHUD::NotifyHitBoxBeginCursorOver(FName BoxName)
{
	Super::NotifyHitBoxBeginCursorOver(BoxName);

	if (Buttons.Contains(BoxName))
		Buttons[BoxName]->NotifyHitBoxBeginCursorOver(this);
}

void AMainHUD::NotifyHitBoxClick(FName BoxName)
{
	Super::NotifyHitBoxClick(BoxName);

	if (Buttons.Contains(BoxName))
		Buttons[BoxName]->NotifyHitBoxClick(this);
}

void AMainHUD::NotifyHitBoxEndCursorOver(FName BoxName)
{
	Super::NotifyHitBoxEndCursorOver(BoxName);

	if (Buttons.Contains(BoxName))
		Buttons[BoxName]->NotifyHitBoxEndCursorOver(this);
}

void AMainHUD::NotifyHitBoxRelease(FName BoxName)
{
	Super::NotifyHitBoxRelease(BoxName);

	if (Buttons.Contains(BoxName))
		Buttons[BoxName]->NotifyHitBoxRelease(this);
}

void AMainHUD::Destroyed()
{
	for (auto & Elem : Buttons)
		delete Elem.Value;
}
