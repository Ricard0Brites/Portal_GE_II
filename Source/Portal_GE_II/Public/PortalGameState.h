// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "PortalManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

#include "PortalGameState.generated.h"

class APortalManager;

UCLASS()
class PORTAL_GE_II_API APortalGameState : public AGameState
{
	GENERATED_BODY()

private:
	APortalManager* asPortalManager;

private:
#pragma region Default
	virtual void BeginPlay() override;
#pragma endregion


#pragma region WeaponProperties
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_WeaponType)
		TArray<FString> sWeaponType;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TArray<FLinearColor> lcWeaponColor;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_WeaponCooldown)
		TArray<float> fWeaponCooldown;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_WeaponAmmo)
		TArray<int32> iWeaponAmmo;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_WeaponDamage)
		TArray<float> fWeaponDamage;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_BulletSpeed)
		TArray<float> fBulletSpeed;
	UPROPERTY(EditDefaultsOnly, category = "Weapon Properties", meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_BulletGravityScale)
		TArray<float> fBulletGravityScale;
#pragma endregion

public:

#pragma region WeaponPropertiesGetters
	FLinearColor GetWeaponColor(int32 weaponTypePayload) { return lcWeaponColor[weaponTypePayload]; }
	int32 GetWeaponAmmoAmount(int32 weaponTypePayload) { return iWeaponAmmo[weaponTypePayload]; }
	float GetPlatformCooldown(int32 weaponTypePayload) { return fWeaponCooldown[weaponTypePayload]; }
	float GetBulletSpeed(int32 weaponTypePayload) { return fBulletSpeed[weaponTypePayload]; }
	float GetBulletGravityScale(int32 weaponTypePayload) { return fBulletGravityScale[weaponTypePayload]; }
	float GetWeaponDamage(int32 weaponTypePayload) { return fWeaponDamage[weaponTypePayload]; }
#pragma endregion

#pragma region Replication
	// Variable replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	//onrep functions
	UFUNCTION()
	void OnRep_BulletGravityScale();
	UFUNCTION()
	void OnRep_BulletSpeed();
	UFUNCTION()
	void OnRep_WeaponDamage();
	UFUNCTION()
	void OnRep_WeaponAmmo();
	UFUNCTION()
	void OnRep_WeaponCooldown();
	UFUNCTION()
	void OnRep_WeaponType();
#pragma endregion

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	int PlayerKills;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	int PlayerDeaths;

	UFUNCTION()
	void IncrementPlayerKills();
};
