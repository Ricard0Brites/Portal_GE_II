// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PortalClass.generated.h"

UCLASS()
class PORTAL_GE_II_API APortalClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		UStaticMeshComponent* portalOuterRing;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		UStaticMeshComponent* portalInnerRing;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		USceneCaptureComponent2D* sceneCapture;
};
