// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

// Sets default values
APortalManager::APortalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld() ? asPlayerCharacter = Cast<APortal_GE_IICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) : 0;
}

void APortalManager::SpawnPortal(bool bPortalType, FVector Location)
{
	if (GetWorld())
	{
		if (bPortalType) //blue portal
		{
			// if the blue portal is active its destroyed
			if (isBluePortalActive)
			{
				//if the object isn't pending kill already, its destroyed
				if (!bluePortalRef->IsPendingKill())
				{
					bluePortalRef->Destroy();
				}
				isBluePortalActive = false;
			}

			//spawns the portal
			bluePortalRef = GetWorld()->SpawnActor<APortalClass>(portalBPRef, Location, GetPortalSpawnRotation());
			//assigns the blue color
			bluePortalRef->SetPortalColor(FLinearColor(.0f, 0.35f, 1.f, 1.f));


			//sets the portal state (avoids multiple blue portals)
			isBluePortalActive = true;
		}
		if (!bPortalType)
		{
			//if the orange portal is active it is destroyed
			if (isOrangePortalActive)
			{
				//if the object isn't pending kill already, its destroyed
				if (!orangePortalRef->IsPendingKill())
				{
					orangePortalRef->Destroy();
				}
				isOrangePortalActive = false;
			}

			//spawns the portal
			orangePortalRef = GetWorld()->SpawnActor<APortalClass>(portalBPRef, Location, GetPortalSpawnRotation());
			//assigns the orange color
			orangePortalRef->SetPortalColor(FLinearColor(1.f, 0.35f, .0f, 1.f));


			//sets the portal state (avoids multiple blue portals)
			isOrangePortalActive = true;
		}
	}
}

FRotator APortalManager::GetPortalSpawnRotation()
{
	return asPlayerCharacter->portalSpawnRotation;
}