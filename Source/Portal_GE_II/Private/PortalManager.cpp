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
		//blue portal
		if (bPortalType) 
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
			//assigns the portal its type
			bluePortalRef->SetPortalType(bPortalType);


			//sets the portal state (avoids multiple blue portals)
			isBluePortalActive = true;
		}
		
		//orange portal
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
			//assigns the portal its type
			orangePortalRef->SetPortalType(bPortalType);

			//sets the portal state (avoids multiple blue portals)
			isOrangePortalActive = true;
		}
	}
}

FRotator APortalManager::GetPortalSpawnRotation()
{
	return asPlayerCharacter->portalSpawnRotation;
}

#pragma region PortalCameraRotation
FRotator APortalManager::CalculateAngleBetweenCharacterAndPortal(APortalClass* portalReference)
{
	// vector u = character location - portal location 
	// formula: u = b - a
	FVector u = asPlayerCharacter->GetActorLocation() - GetActorLocation();
	// v = portal right vector
	FVector v = portalReference->GetActorRightVector();

	// uses a worked version of the formula u.v = |u|.|v| X cos(u ^ v)
	return FRotator(0, 0, UKismetMathLibrary::Acos((FVector::DotProduct(u, v)) / (GetVectorLength(u) * GetVectorLength(v))));
}

void APortalManager::SetBluePortalCameraRotation(FRotator payload)
{
	if (bluePortalRef)
	{
		bluePortalRef->sceneCapture->SetRelativeRotation(FRotator(0, payload.Yaw, 0));
	}
}

void APortalManager::SetOrangePortalCameraRotation(FRotator payload)
{
	if (orangePortalRef)
	{
		orangePortalRef->sceneCapture->SetRelativeRotation(FRotator(0, payload.Yaw, 0));
	}
}
#pragma endregion

#pragma region Teleportation
void APortalManager::TeleportCharacter(APortalClass* inPortalRef)
{
	//save player velocity
	FVector playerVelocity = asPlayerCharacter->GetVelocity();
	//save player rotation
	FRotator playerRotation = asPlayerCharacter->GetActorRotation();
	//calculate and saves the players location relative to the portal
	FVector playerLocationRelativeToPortal = asPlayerCharacter->GetActorLocation() - inPortalRef->GetActorLocation();

	float portalRotationOffset = (bluePortalRef->GetActorRotation().Yaw - orangePortalRef->GetActorRotation().Yaw);
	

	//blue portal traveling to orange
	if (inPortalRef->GetPortalType())
	{
		asPlayerCharacter->SetActorLocation(orangePortalRef->GetActorLocation() + 
			((orangePortalRef->GetActorForwardVector() + orangePortalRef->GetActorRightVector()) *
				playerLocationRelativeToPortal));

		asPlayerCharacter->SetActorRotation(FRotator(0, playerRotation.Yaw + portalRotationOffset, 0));

	}
	//orange portal traveling to blue
	else
	{
		asPlayerCharacter->SetActorLocation(bluePortalRef->GetActorLocation());
		asPlayerCharacter->AddActorWorldOffset(playerLocationRelativeToPortal);

		asPlayerCharacter->SetActorRotation(FRotator(0, playerLocationRelativeToPortal.Z, 0));
	}

}
#pragma endregion


#pragma region Math
float APortalManager::GetVectorLength(FVector payload)
{
	return UKismetMathLibrary::Sqrt((UKismetMathLibrary::Square(payload.X)) + (UKismetMathLibrary::Square(payload.Y)) + (UKismetMathLibrary::Square(payload.Z)));
}

#pragma endregion


