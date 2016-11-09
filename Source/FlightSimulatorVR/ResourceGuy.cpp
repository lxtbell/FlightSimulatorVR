// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightSimulatorVR.h"
#include "ResourceGuy.h"
#include "Public/EngineUtils.h"


// Sets default values
AResourceGuy::AResourceGuy()
{
}

class AResourceGuy* GetFirstResourceGuy(class UWorld* World)
{
	TActorIterator<AResourceGuy> ActorItr(World);
	if (ActorItr)
		return *ActorItr;

	return nullptr;
}
