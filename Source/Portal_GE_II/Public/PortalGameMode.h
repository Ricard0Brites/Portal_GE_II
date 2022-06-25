// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PortalGameMode.generated.h"

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
			TArray<int16> iWeaponAmmo;
};
