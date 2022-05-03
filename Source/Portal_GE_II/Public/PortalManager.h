// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalClass.h"
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

#pragma region PortalSpawning

	//portal blueprint reference (to spawn later on)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Portal", meta = (allowprivateaccess = true))
		TSubclassOf<APortalClass> portalBPRef;
public:
	/*
	* PortalType = True ----> Blue portal
	* PortalType = False ---> Orange Portal
	
	*/
	static void SpawnPortal(bool bPortalType);
#pragma endregion

};
