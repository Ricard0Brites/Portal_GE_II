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

#pragma region Components
protected:
	// Mesh Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		UStaticMeshComponent* portalOuterRing;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		UStaticMeshComponent* portalInnerRing;

public:
	//Scene Capture Component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		USceneCaptureComponent2D* sceneCapture;
#pragma endregion

#pragma region SpawnParameters
private:
	//this variable sets the portal color
	FLinearColor portalColor;


public:

	UFUNCTION(BlueprintCallable, Category = "Portal")
		FLinearColor GetPortalColor() { return portalColor; };

	void SetPortalColor(FLinearColor color) { portalColor = color; };
#pragma endregion

};
