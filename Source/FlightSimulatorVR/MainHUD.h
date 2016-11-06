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

	class APilotState* PilotState;

	struct UButton
	{
		FName ButtonName; FLinearColor ButtonNormalColor, ButtonHighlightColor;
		FVector2D ButtonScreenLocation; UFont* ButtonFont; float ButtonFontScale; FVector2D ButtonHitBoxScale; 
		FLinearColor CurrentColor;

		UButton* Set(FName Name, FLinearColor NormalColor, FLinearColor HighlightColor)
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

		UButton* Draw(class AMainHUD* HUD)
		{
			HUD->DrawText(ButtonName.ToString(), CurrentColor, ButtonScreenLocation.X, ButtonScreenLocation.Y, ButtonFont, ButtonFontScale);
			HUD->AddHitBox(ButtonScreenLocation, ButtonHitBoxScale, ButtonName, true);

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
