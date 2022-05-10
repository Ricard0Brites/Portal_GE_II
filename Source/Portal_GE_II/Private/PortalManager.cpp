// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "EngineMinimal.h"
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
	//save player velocity
	FVector playerVelocity = asPlayerCharacter->GetVelocity();

	//calculates the angle between the portals
	float angleBetweenPortalsPercent = UKismetMathLibrary::Acos(
		(FVector::DotProduct(bluePortalRef->GetActorLocation(), orangePortalRef->GetActorLocation())) 
		/ 
		(  (GetVectorLength(bluePortalRef->GetActorLocation())) * (GetVectorLength(orangePortalRef->GetActorLocation())))) / 180;
	

	//blue portal traveling to orange
	if (inPortalRef->GetPortalType())
	{
		//set player rotation
			asPlayerCharacter->GetController()->SetControlRotation(
				FRotator(0, 
					ConvertRotationToActorSpace(inPortalRef->GetActorRotation(), inPortalRef, orangePortalRef).Yaw,
					0));
			//set player location
			asPlayerCharacter->SetActorLocation(orangePortalRef->GetActorLocation());
			//set player velocity
			asPlayerCharacter->movementComp->Velocity = FVector( 
				UKismetMathLibrary::Lerp(playerVelocity.X, playerVelocity.Y, UKismetMathLibrary::Clamp(angleBetweenPortalsPercent, 0, 1)), // X
				UKismetMathLibrary::Lerp(playerVelocity.Y, playerVelocity.X, UKismetMathLibrary::Clamp(angleBetweenPortalsPercent, 0, 1)), // Y
				playerVelocity.Z);
	}
	//orange portal traveling to blue
	else
	{
		//set player rotation
		asPlayerCharacter->GetController()->SetControlRotation(
			FRotator(0,
				ConvertRotationToActorSpace(inPortalRef->GetActorRotation(), inPortalRef, bluePortalRef).Yaw,
				0));
		//set player location
		asPlayerCharacter->SetActorLocation(bluePortalRef->GetActorLocation());
		//set player velocity
		asPlayerCharacter->movementComp->Velocity = FVector(
			UKismetMathLibrary::Lerp(playerVelocity.X, playerVelocity.Y, UKismetMathLibrary::Clamp(angleBetweenPortalsPercent, 0, 1)), // X
			UKismetMathLibrary::Lerp(playerVelocity.Y, playerVelocity.X, UKismetMathLibrary::Clamp(angleBetweenPortalsPercent, 0, 1)), // Y 
			playerVelocity.Z);
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

//converts actor world rotation to relative to another actor
FVector APortalManager::ConvertLocationToActorSpace(FVector Location, AActor* Reference, AActor* Target)
{
	if (Reference == nullptr || Target == nullptr)
	{
		return FVector::ZeroVector;
	}

	FVector Direction = Location - Reference->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FVector Dots;
	Dots.X = FVector::DotProduct(Direction, Reference->GetActorForwardVector());
	Dots.Y = FVector::DotProduct(Direction, Reference->GetActorRightVector());
	Dots.Z = FVector::DotProduct(Direction, Reference->GetActorUpVector());

	FVector NewDirection = Dots.X * Target->GetActorForwardVector()
		+ Dots.Y * Target->GetActorRightVector()
		+ Dots.Z * Target->GetActorUpVector();

	return TargetLocation + NewDirection;
}
// converts actor world rotation to relative rotation to another actor
FRotator APortalManager::ConvertRotationToActorSpace(FRotator Rotation, AActor* Reference, AActor* Target)
{
	if (Reference == nullptr || Target == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	FTransform SourceTransform = Reference->GetActorTransform();
	FTransform TargetTransform = Target->GetActorTransform();
	FQuat QuatRotation = FQuat(Rotation);

	FQuat LocalQuat = SourceTransform.GetRotation().Inverse() * QuatRotation;
	FQuat NewWorldQuat = TargetTransform.GetRotation() * LocalQuat;

	return NewWorldQuat.Rotator();
}
#pragma endregion


