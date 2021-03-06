// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

//#include "../Portal_GE_IICharacter.h"
//#include "././Kismet/GameplayStatics.h"
//#include "././Kismet/KismetSystemLibrary.h"

#include "PortalGameMode.generated.h"

class APortal_GE_IICharacter;
class UGameplayStatics;
/**
 * 
 */
UCLASS(MinimalAPI)
class APortalGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APortalGameMode();
	void Respawn(AController* Cont);

};
