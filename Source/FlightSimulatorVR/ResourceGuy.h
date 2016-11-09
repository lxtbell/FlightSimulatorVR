// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ResourceGuy.generated.h"


UCLASS()
class FLIGHTSIMULATORVR_API AResourceGuy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceGuy();

	UPROPERTY(Category = AMainHUD, EditAnywhere)
	class UTexture* AMainHUD_MenuBackground;

	UPROPERTY(Category = AMainHUD, EditAnywhere)
	class UFont* AMainHUD_MenuFont;

	UPROPERTY(Category = AMainHUD, EditAnywhere)
	class UFont* AMainHUD_WastedFont;

	UPROPERTY(Category = ATargets, EditAnywhere)
	class UStaticMesh* ATargets_Cube;

	UPROPERTY(Category = ATargets, EditAnywhere)
	class UMaterial* ATargets_Glass;
};

class AResourceGuy* GetFirstResourceGuy(class UWorld* World);
