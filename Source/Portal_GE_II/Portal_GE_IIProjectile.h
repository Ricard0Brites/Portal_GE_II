// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PortalManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Portal_GE_IIProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class APortal_GE_IIProjectile : public AActor
{
	GENERATED_BODY()

#pragma region Components
		/** Sphere collision component */
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
#pragma region CastingReferences
	//the reference to the Subclass
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Portal", meta = (allowprivateaccess = true))
		TSubclassOf<APortalManager> portalManagerBpRef;
	// the reference to the actual portal BP
	APortalManager* asPortalManager;
#pragma endregion

public:
	APortal_GE_IIProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

#pragma region DefaultFunctions
	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
#pragma endregion


#pragma region PortalParameters
	// Saves which type of portal should spawn
	bool bPortalTypeToSpawn;
#pragma endregion


};

