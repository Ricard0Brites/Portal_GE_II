// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGameMode.h"

class UGameplayStatics;

void APortalGameMode::SR_SpawnWeaponInPlayer(TSubclassOf<APortal_GE_IICharacter> characterRef,int32 weaponType)
{
	////// Create a gun mesh component
	//characterRef->FP_Gun = characterRef.CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//asCharacter.FP_Gun->SetOnlyOwnerSee(true);			// otherwise won't be visible in the multiplayer
	//asCharacter.FP_Gun->bCastDynamicShadow = false;
	//asCharacter.FP_Gun->CastShadow = false;
	//asCharacter.FP_Gun->SetupAttachment(asCharacter.GetMesh1P(), TEXT("GripPoint"));
	//asCharacter.FP_Gun->SetupAttachment(asCharacter.GetRootComponent());

	////allows the character to shoot again
	//asCharacter.SetCanShoot(true);

	////pass the weapon type to the gun
	//asCharacter.SetWeaponType(weaponType);

	////trigger a color change in the 3p gun mesh
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
}

FLinearColor APortalGameMode::GetWeaponColor(int32 weaponType)
{
	return lcWeaponColor[weaponType];
}