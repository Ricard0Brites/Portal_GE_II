// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalClass.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Portal_GE_IICharacter.h"
#include "PortalManager.generated.h"

UCLASS()
class PORTAL_GE_II_API APortalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region CastingReferences
	//a reference to the player character blueprint
	APortal_GE_IICharacter* asPlayerCharacter;
#pragma endregion


#pragma region PortalSpawning

	//portal blueprint reference (to spawn later on)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Portal", meta = (allowprivateaccess = true))
		TSubclassOf<APortalClass> portalBPRef;

public:
	/*
	* Spawns a portal
	* 
	* @param bPortalType ---> the type of portal to spawn
	* @param Location ------> the location where the portal should spawn
	* 
	* PortalType = True ----> Blue portal
	* PortalType = False ---> Orange Portal
	*/
	void SpawnPortal(bool bPortalType, FVector Location);

	/*
	* Portal Rotation Getter
	*/
	FRotator GetPortalSpawnRotation();
#pragma endregion

private:
#pragma region PortalState

	// this variable keeps the state of the portal
	bool isBluePortalActive = false, isOrangePortalActive = false;

public:
	UFUNCTION(BlueprintPure, Category = "Portal State")
		bool GetIsBluePortalActive() { return isBluePortalActive; };
	UFUNCTION(BlueprintPure, Category = "Portal State")
		bool GetIsOrangePortalActive() { return isOrangePortalActive; };

#pragma endregion

#pragma region PortalColor

	//this variable keeps the portal reference
	APortalClass* bluePortalRef, * orangePortalRef;

#pragma endregion


public:
#pragma region ActorReferences
	//actor reference getters
	UFUNCTION(BlueprintPure, Category = "Portal")
		APortalClass* GetBluePortalRef() { return bluePortalRef; };
	UFUNCTION(BlueprintPure, Category = "Portal")
		APortalClass* GetOrangePortalRef() { return orangePortalRef; };
#pragma endregion

#pragma region PortalCameraRotation
private:
	FRotator bluePortalCameraRotation, orangePortalCameraRotation;
public:

	//calculates the angle between the player and the portal
	FRotator CalculateAngleBetweenCharacterAndPortal(APortalClass* portalReference);

	//blue portal getter and setter
	void SetBluePortalCameraRotation(FRotator payload);
	FRotator GetBluePortalCameraRotation() { return bluePortalCameraRotation; };


	//orange portal getter and setter
	void SetOrangePortalCameraRotation(FRotator payload);
	FRotator GetOrangePortalCameraRotation() { return orangePortalCameraRotation; };
#pragma endregion

#pragma region Math
	float GetVectorLength(FVector payload);
#pragma endregion

#pragma region Teleportation
	/*
	* teleports player to the correspondent portal
	*/
	void TeleportCharacter(APortalClass* inPortalRef);
#pragma endregion

};
