// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Portal_GE_IIHUD.generated.h"

UCLASS()
class APortal_GE_IIHUD : public AHUD
{
	GENERATED_BODY()

public:
	APortal_GE_IIHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

