// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGameState.h"

void APortalGameState::SpawnPortalOnAllClients_Implementation(FVector location, bool portalType, APortalManager* portalManagerRef, FHitResult hit)
{
		portalManagerRef->SpawnPortal(portalType, location, hit);
}

