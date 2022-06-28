// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PortalGameMode.h"

#include "WeaponPlatform.generated.h"

UCLASS()
class PORTAL_GE_II_API AWeaponPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh",meta = ( AllowPrivateAccess = true ))
		UStaticMeshComponent* platformMesh;
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = ( AllowPrivateAccess = true ))
		UStaticMeshComponent* hologramMesh;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Visuals", meta	= ( AllowPrivateAccess = true ))
		float fHologramRotationSpeed;
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
		UBoxComponent* boxCollider;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Object Type", meta = (AllowPrivateAccess = true))
		int32 iObjectType;
	APortalGameMode* asGameMode;
	UFUNCTION()
		virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void ChangeGunMeshColor(FLinearColor ColorToChangeTo);
};
