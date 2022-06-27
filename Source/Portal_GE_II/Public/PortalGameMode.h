// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "../Portal_GE_IICharacter.h"
#include "././Kismet/GameplayStatics.h"
#include "././Kismet/KismetSystemLibrary.h"

#include "PortalGameMode.generated.h"

class APortal_GE_IICharacter;
class UGameplayStatics;
/**
 * 
 */
UCLASS()
class PORTAL_GE_II_API APortalGameMode : public AGameMode
{
	GENERATED_BODY()

private:
		
		UPROPERTY( EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
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
