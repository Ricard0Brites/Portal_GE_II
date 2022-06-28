// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PortalManager.h"
#include "Public/PortalGameMode.h"
#include "PortalGameState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Portal_GE_IIProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class APortalManager;
class APortalGameState;

UCLASS(config=Game)
class APortal_GE_IIProjectile : public AActor
{
	GENERATED_BODY()



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	APortal_GE_IIProjectile();

public:
#pragma region CastingReferences
	//the reference to the Subclass
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Portal", meta = (AllowPrivateAccess = true))
		TSubclassOf<APortalManager> portalManagerBpRef;
	// the reference to the actual portal BP
	APortalManager* asPortalManager;

	//reference to the game state class
	UPROPERTY(BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = true))
		AActor* gameStateRef;

	APortalGameState* asGameState;

	APortalGameMode* asGameMode;
#pragma endregion

protected:
#pragma region Components
		/** Sphere collision component */
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;
#pragma endregion

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
public:
	// Saves which type of portal should spawn
	bool bPortalTypeToSpawn;
	bool bCanPortalSpawn = false;
#pragma endregion

#pragma region MyParameters
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	float fDamage;
public:
	void SetDamage(float Val) { fDamage = Val; }
#pragma endregion


#pragma region Multiplayer
private:
	UFUNCTION( Server, Reliable, BlueprintCallable )
		void SR_SpawnPortals(FVector location, bool portalType, FHitResult hitResult);
	UFUNCTION(Server, Reliable)
		void SR_GetBulletParams(int32 weaponTypeIndexPayload);
public:
	void SetBulletParameters(int32 weaponTypeIndexPayload);
#pragma endregion
};

