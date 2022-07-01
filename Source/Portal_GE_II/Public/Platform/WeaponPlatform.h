// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PortalGameState.h"

#include "WeaponPlatform.generated.h"

UCLASS()
class PORTAL_GE_II_API AWeaponPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPlatform();

protected:
#pragma region Default
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region References
	APortalGameState* asGameState;
#pragma endregion

#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* platformMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* hologramMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float fHologramRotationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
		UBoxComponent* boxCollider;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Type")
		int32 iObjectType;


#pragma endregion


	UFUNCTION(BlueprintImplementableEvent)
		void ChangeGunMeshColor(FLinearColor ColorToChangeTo);

	UFUNCTION()
		virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
		void EnableHologramComponent();

	UFUNCTION(BlueprintImplementableEvent)
		void DisableHologramComponent();

	UFUNCTION(Server, Reliable)
		void SR_DisableHologram();

	UFUNCTION(Server, Reliable)
		void SR_EnableHologram(AWeaponPlatform* weaponPlatformRefPayload);

public:
	UFUNCTION(NetMulticast, Reliable)
		void MC_DisableAllHolograms();

	UFUNCTION(NetMulticast, Reliable)
		void MC_EnableAllHolograms(AWeaponPlatform* weaponPlatformRefPayload);

	void StartTimer(float timeInSeconds);

};
