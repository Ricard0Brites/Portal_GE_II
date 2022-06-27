// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "PortalManager.h"

#include "PortalGameState.generated.h"

class APortalManager;

UCLASS()
class PORTAL_GE_II_API APortalGameState : public AGameState
{
	GENERATED_BODY()

private:
	APortalManager* asPortalManager;

public:
		UFUNCTION(NetMulticast, Unreliable)
			void SpawnPortalOnAllClients(FVector location, bool portalType, APortalManager* portalManagerRef, FHitResult hit);
};
