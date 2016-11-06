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
	
public:
	AMainHUD();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;
};
