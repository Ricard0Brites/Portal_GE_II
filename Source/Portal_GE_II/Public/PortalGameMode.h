// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "../Portal_GE_IICharacter.h"
#include "././Kismet/GameplayStatics.h"

#include "PortalGameMode.generated.h"

class APortal_GE_IICharacter;
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


		
public:
	/*
	* weapon type: 
	*	0-> Assault Rifle
	*	1-> Shotgun
	*	2-> Rocket Launcher
	*	3-> Portal Gun
	*/
	UFUNCTION( BlueprintCallable )
	void SR_SpawnWeaponInPlayer(TSubclassOf<APortal_GE_IICharacter> characterRef, int32 weaponType);
	
	FLinearColor GetWeaponColor(int32 weaponType);

};
