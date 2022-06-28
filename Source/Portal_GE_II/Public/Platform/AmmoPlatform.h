// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Platform/WeaponPlatform.h"
#include "AmmoPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PORTAL_GE_II_API AAmmoPlatform : public AWeaponPlatform
{
	GENERATED_BODY()
	
		virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
