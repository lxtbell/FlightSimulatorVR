// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"


UCLASS()
class FLIGHTSIMULATORVR_API AMainHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(Category = Volume, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTexture* MenuBackground;

	UPROPERTY(Category = Volume, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFont* MenuFont;

	UPROPERTY(Category = Volume, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFont* WastedFont;
	
public:
	AMainHUD();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostRender() override;

	virtual void NotifyHitBoxBeginCursorOver(FName BoxName) override;

	virtual void NotifyHitBoxClick(FName BoxName) override;

	virtual void NotifyHitBoxEndCursorOver(FName BoxName) override;

	virtual void NotifyHitBoxRelease(FName BoxName) override;

	virtual void Destroyed() override;

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

	struct UButton
	{
		FString ButtonName; FLinearColor ButtonNormalColor, ButtonHighlightColor;
		FVector2D ButtonScreenLocation; UFont* ButtonFont; float ButtonFontScale; FVector2D ButtonHitBoxScale; 
		FLinearColor CurrentColor;

		enum class Alignment
		{
			TopLeft,
			Center,
			BottomRight
		};

		UButton* Set(const FString & Name, const FLinearColor & NormalColor, const FLinearColor & HighlightColor)
		{
			ButtonName = Name; ButtonNormalColor = NormalColor; ButtonHighlightColor = HighlightColor;
			CurrentColor = ButtonNormalColor;

			return this;
		}
		
		UButton* Set(float ScreenX, float ScreenY, UFont* Font, float FontScale, const FVector2D & HitBoxScale)
		{
			ButtonScreenLocation = FVector2D(ScreenX, ScreenY); ButtonFont = Font; ButtonFontScale = FontScale; ButtonHitBoxScale = HitBoxScale;

			return this;
		}

		UButton* Draw(class AMainHUD* HUD, Alignment TextAlignment = Alignment::TopLeft)
		{
			FVector2D ScreenLocation = ButtonScreenLocation;
			switch (TextAlignment)
			{
			case Alignment::Center:
				ScreenLocation -= ButtonHitBoxScale / 2;
				break;
			case Alignment::BottomRight:
				ScreenLocation -= ButtonHitBoxScale;
				break;
			}

			if (FMath::Rand() % 500 == 0)
				UE_LOG(LogTemp, Warning, TEXT("AMainHUD::UButton::Draw %s"), *ButtonName);

			HUD->DrawText(ButtonName, CurrentColor, ScreenLocation.X, ScreenLocation.Y, ButtonFont, ButtonFontScale);
			HUD->AddHitBox(ScreenLocation, ButtonHitBoxScale, FName(*ButtonName), true);

			return this;
		}

		virtual void NotifyHitBoxBeginCursorOver(class AMainHUD* HUD)
		{
			CurrentColor = ButtonHighlightColor;
		}

		virtual void NotifyHitBoxClick(class AMainHUD* HUD)
		{
		}

		virtual void NotifyHitBoxEndCursorOver(class AMainHUD* HUD)
		{
			CurrentColor = ButtonHighlightColor;
		}

		virtual void NotifyHitBoxRelease(class AMainHUD* HUD)
		{
		}
	};

	TMap<FName, UButton*> Buttons;

	struct UButtonStartGame : public UButton
	{
		virtual void NotifyHitBoxClick(class AMainHUD* HUD) override;
	};

	struct UButtonStartWithTutorial : public UButton
	{
		virtual void NotifyHitBoxClick(class AMainHUD* HUD) override;
	};

	struct UButtonExitGame : public UButton
	{
		virtual void NotifyHitBoxClick(class AMainHUD* HUD) override;
	};
};
