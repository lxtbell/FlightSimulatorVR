// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "MainHUD.h"
#include "Public/EngineUtils.h"
#include "FlightSimulatorVRPawn.h"


AMainHUD::AMainHUD()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> MenuBackground;
		FConstructorStatics() :
			MenuBackground(TEXT("/Game/Interface/New_York_Ruins.New_York_Ruins"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	MenuBackground = ConstructorStatics.MenuBackground.Get();
}

// Called when the game starts or when spawned
void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	APawn *Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Pawn->IsA(AFlightSimulatorVRPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AFlightSimulatorVRPawn::StartFlying %s"), *Pawn->GetName());

		Cast<AFlightSimulatorVRPawn>(Pawn)->StartFlying();
	}
}

void AMainHUD::DrawHUD()
{
	Super::DrawHUD();
}
