// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform/AmmoPlatform.h"

void AAmmoPlatform::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APortal_GE_IICharacter* asCharacter = Cast<APortal_GE_IICharacter>(OtherActor);
	if (asCharacter != nullptr && HasAuthority())
	{
		//requests ammo if its the server  (there is no need to request on the client -----> the value will update Automatically because of the onrep notify variable  )
		if (asCharacter->GetWeaponType() == iObjectType && asCharacter->GetCharacterHasWeapon() && iObjectType != asCharacter->GetPortalGunIndex())
		{
			asCharacter->RequestAmmo(iObjectType);
			asCharacter->SetCanShoot(true);
			
			if (HasAuthority())
			{
				MC_DisableAllHolograms();
				MC_EnableAllHolograms(this);
			}
			else
			{
				SR_DisableHologram();
				SR_EnableHologram(this);
			}
		}
	}
}
