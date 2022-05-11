// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "EngineMinimal.h"
#include "Math/Rotator.h"
#include "GameFramework/MovementComponent.h"


class UCharacterMovementComponent;

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

#pragma region PortalSpawn
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
#pragma endregion

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
		bluePortalRef->sceneCapture->SetRelativeRotation(FRotator(payload.Pitch, payload.Yaw, 0));
	}
}

void APortalManager::SetOrangePortalCameraRotation(FRotator payload)
{
	if (orangePortalRef)
	{
		orangePortalRef->sceneCapture->SetRelativeRotation(FRotator(payload.Pitch, payload.Yaw, 0));
	}
}
#pragma endregion

#pragma region Teleportation
void APortalManager::TeleportCharacter(APortalClass* inPortalRef)
{
	FVector velocity = asPlayerCharacter->GetVelocity();
	if (inPortalRef == bluePortalRef)
	{
		//location
		asPlayerCharacter->SetActorLocation(orangePortalRef->GetActorLocation());

		//rotation
		FRotator approxOrangePortalRot = UKismetMathLibrary::MakeRotFromXY(orangePortalRef->GetActorForwardVector(), orangePortalRef->GetActorRightVector());
		approxOrangePortalRot.Pitch = 0; 
		approxOrangePortalRot.Roll = 0;
		approxOrangePortalRot.Yaw = UKismetMathLibrary::Round(approxOrangePortalRot.Yaw);
		FRotator playerRot = FRotator(0, asPlayerCharacter->GetActorRotation().Yaw, 0);

		UKismetMathLibrary::Abs(orangePortalRef->GetActorForwardVector().X) == 1 ? approxOrangePortalRot *= 4 * orangePortalRef->GetActorForwardVector().X : approxOrangePortalRot *= 4 * orangePortalRef->GetActorForwardVector().Y;

		if (UKismetMathLibrary::Abs(approxOrangePortalRot.Yaw) != 90)
		{
			approxOrangePortalRot.Yaw *= UKismetMathLibrary::Sin(30 - approxOrangePortalRot.Yaw);
			asPlayerCharacter->movementComp->StopMovementImmediately();
		}
		asPlayerCharacter->GetController()->SetControlRotation((approxOrangePortalRot) + playerRot);

		//Velocity

		asPlayerCharacter->movementComp->Velocity = (asPlayerCharacter->GetActorForwardVector() * velocity ) * orangePortalRef->GetActorForwardVector();
	}
	else
	{
		//location
		asPlayerCharacter->SetActorLocation(bluePortalRef->GetActorLocation());

		//rotation
		FRotator approxBluePortalRot = UKismetMathLibrary::MakeRotFromXY(bluePortalRef->GetActorForwardVector(), bluePortalRef->GetActorRightVector());
		approxBluePortalRot.Pitch = 0;
		approxBluePortalRot.Roll = 0;
		approxBluePortalRot.Yaw = UKismetMathLibrary::Round(approxBluePortalRot.Yaw);
		FRotator playerRot = FRotator(0, asPlayerCharacter->GetActorRotation().Yaw, 0);

		UKismetMathLibrary::Abs(bluePortalRef->GetActorForwardVector().X) == 1 ? approxBluePortalRot *= 4 * bluePortalRef->GetActorForwardVector().X : approxBluePortalRot *= 4 * bluePortalRef->GetActorForwardVector().Y;

		if (UKismetMathLibrary::Abs(approxBluePortalRot.Yaw) != 90)
		{
			approxBluePortalRot.Yaw *= UKismetMathLibrary::Sin(30 - approxBluePortalRot.Yaw);
			asPlayerCharacter->movementComp->StopMovementImmediately();
		}
		asPlayerCharacter->GetController()->SetControlRotation((approxBluePortalRot)+playerRot);


		//velocity
		asPlayerCharacter->movementComp->Velocity =(asPlayerCharacter->GetActorForwardVector() * velocity) * bluePortalRef->GetActorForwardVector();
	}

}

//sets the other portals bool to avoid an endless loop of Teleportation
void APortalManager::SetOtherCanTeleport(APortalClass* portalRef, bool payload)
{
	if (portalRef == bluePortalRef)
	{
		orangePortalRef->SetCanTeleport(payload);
	}
	else
	{
		bluePortalRef->SetCanTeleport(payload);
	}
	
}

#pragma endregion

#pragma region Math
float APortalManager::GetVectorLength(FVector payload)
{
	return UKismetMathLibrary::Sqrt((UKismetMathLibrary::Square(payload.X)) + (UKismetMathLibrary::Square(payload.Y)) + (UKismetMathLibrary::Square(payload.Z)));
}
#pragma endregion


