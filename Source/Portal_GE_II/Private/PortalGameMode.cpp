// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGameMode.h"



void APortalGameMode::SR_SpawnWeaponInPlayer_Implementation(APortal_GE_IICharacter* charRef, int32 weaponTypePayload)
{	
	//allows the character to shoot again
	(*charRef).SetCanShoot(true);

	//pass the weapon type to the gun
	(*charRef).SetWeaponType(weaponTypePayload);

	//trigger a color change in the 3p gun mesh
	
}

FLinearColor APortalGameMode::GetWeaponColor(int32 weaponType)
{
	return lcWeaponColor[weaponType];
}