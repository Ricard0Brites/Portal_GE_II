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
			bluePortalRef = GetWorld()->SpawnActor<APortalClass>(portalBPRef, Location, GetPortalSpawnRotation());
			bluePortalRef->SetPortalColor(FLinearColor(.0f, 0.35f, 1.f, 1.f));
		}
		else if (!bPortalType)
		{
			orangePortalRef = GetWorld()->SpawnActor<APortalClass>(portalBPRef, Location, GetPortalSpawnRotation());
			orangePortalRef->SetPortalColor(FLinearColor(1.f, 0.35f, .0f, 1.f));
		}
	}
}

FRotator APortalManager::GetPortalSpawnRotation()
{
	return asPlayerCharacter->portalSpawnRotation;
}

