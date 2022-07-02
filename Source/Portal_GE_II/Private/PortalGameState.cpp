// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGameState.h"

#pragma region Default

void APortalGameState::BeginPlay()
{
	Super::BeginPlay();

	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		//updates the clients version of the values to be coherent and avoid cheats
		OnRep_BulletGravityScale();
		OnRep_BulletSpeed();
		OnRep_WeaponDamage();
		OnRep_WeaponAmmo();
		OnRep_WeaponCooldown();
		OnRep_WeaponType();
	}
}

#pragma endregion

#pragma region Replication

void APortalGameState::OnRep_BulletGravityScale()
{
	//logs
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::fBulletGravityScale[]"));
}

void APortalGameState::OnRep_BulletSpeed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::fBulletSpeed[]"));
}

void APortalGameState::OnRep_WeaponDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::fWeaponDamage[]"));
}

void APortalGameState::OnRep_WeaponAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::iWeaponAmmo[]"));
}

void APortalGameState::OnRep_WeaponCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::fWeaponCooldown[]"));
}

void APortalGameState::OnRep_WeaponType()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::iWeaponType[]"));
}

void APortalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APortalGameState, sWeaponType);
	DOREPLIFETIME(APortalGameState, fWeaponCooldown);
	DOREPLIFETIME(APortalGameState, iWeaponAmmo);
	DOREPLIFETIME(APortalGameState, fWeaponDamage);
	DOREPLIFETIME(APortalGameState, fBulletSpeed);
	DOREPLIFETIME(APortalGameState, fBulletGravityScale);
}

#pragma endregion
