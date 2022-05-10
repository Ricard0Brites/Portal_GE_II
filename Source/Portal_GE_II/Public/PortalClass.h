// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "../Public/PortalManager.h"
#include "../Portal_GE_IICharacter.h"
#include "../Portal_GE_IIProjectile.h"
#include "PortalClass.generated.h"

class APortalManager;

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
	virtual void Tick(float deltaTime) override;

#pragma region Casting
	APortal_GE_IICharacter* asPlayerCharacter = nullptr;
	
	UPROPERTY(EditAnywhere, category = "Casting", meta = (allowprivateaccess = true))
		TSubclassOf<APortalManager> portalManagerBpRef;
	APortalManager* asPortalManager;

#pragma endregion

#pragma region Components
protected:
	// Mesh Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		UStaticMeshComponent* portalOuterRing;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		UStaticMeshComponent* portalInnerRing;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		UCapsuleComponent* capsuleComponent;

public:
	//Scene Capture Component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (blueprintprotected = true))
		USceneCaptureComponent2D* sceneCapture;
#pragma endregion

#pragma region SpawnParameters
private:

	/*
	* true = Blue
	* false = orange
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal Parameters", meta = (allowprivateaccess = true))
		bool myPortalType;
public:
	void SetPortalType(bool type) { myPortalType = type; };
	bool GetPortalType() { return myPortalType; };
private:

	//this variable sets the portal color
	FLinearColor portalColor;


public:

	UFUNCTION(BlueprintCallable, Category = "Portal")
		FLinearColor GetPortalColor() { return portalColor; };

	void SetPortalColor(FLinearColor color) { portalColor = color; };
#pragma endregion

#pragma region Teleportation
	
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32
			OtherBodyIndex);

private:
	bool canTeleport = true;

public:
	void SetCanTeleport(bool payload) { canTeleport = payload; };

#pragma endregion

};
