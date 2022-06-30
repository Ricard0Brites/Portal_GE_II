// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "PortalManager.h"

#include "PortalGameState.generated.h"

class APortalManager;

UCLASS()
class PORTAL_GE_II_API APortalGameState : public AGameState
{
	GENERATED_BODY()

private:
	APortalManager* asPortalManager;

public:
		UFUNCTION(NetMulticast, Unreliable)
			void SpawnPortalOnAllClients(FVector location, bool portalType, APortalManager* portalManagerRef, FHitResult hit);

private:

	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<FString> sWeaponType;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<FLinearColor> lcWeaponColor;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<float> fWeaponCooldown;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<int32> iWeaponAmmo;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<float> fWeaponDamage;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<float> fBulletSpeed;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<float> fBulletGravityScale;



public:

	FLinearColor GetWeaponColor(int32 weaponTypePayload) { return lcWeaponColor[weaponTypePayload]; }
	int32 GetWeaponAmmoAmount(int32 weaponTypePayload) { return iWeaponAmmo[weaponTypePayload]; }
	float GetPlatformCooldown(int32 weaponTypePayload) { return fWeaponCooldown[weaponTypePayload]; }
	float GetBulletSpeed(int32 weaponTypePayload) { return fBulletSpeed[weaponTypePayload]; }
	float GetBulletGravityScale(int32 weaponTypePayload) { return fBulletGravityScale[weaponTypePayload]; }
	float GetWeaponDamage(int32 weaponTypePayload) { return fWeaponDamage[weaponTypePayload]; }
};
