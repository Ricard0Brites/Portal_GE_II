// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/MovementComponent.h"

#include "Public/PortalGameMode.h"

#include "Portal_GE_IICharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

class APortalGameMode;

UCLASS(config=Game)
class APortal_GE_IICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APortal_GE_IICharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

public:
#pragma region Default
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<class APortal_GE_IIProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

#pragma endregion

#pragma region Components
private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* Mesh1P;

	/** Pawn mesh: 3rd person view (full mesh ) */
	UPROPERTY(BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* Mesh3P;
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* FP_Gun;
public:
	USkeletalMeshComponent* GetMesh1P() { return Mesh1P; }
	USceneComponent* GetRootComponent() { return RootComponent; }

private:
	/** Gun mesh: 3st person view (seen only by self) */
	UPROPERTY(BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* FP_Gun3P;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;
public:
	UPROPERTY(BlueprintReadWrite)
		UMovementComponent* movementComp;
#pragma endregion

protected:
#pragma region Inputs
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Fires a projectile or a portal */
	void OnFireLeft();
	/** Fires a projectile or a portal */
	void OnFireRight();


	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	bool bCanShoot;
	bool bCanShootPortal;
#pragma endregion

public:
#pragma region DefaultFunctions
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
#pragma endregion

private:
#pragma region PortalCrosshairCheck

	/** 
	*	checks if the player can spawn a portal
	* 
	*	@param fLinecastLength is the length the line cast should be
	*	@param sTag is the tag that the cast is searching for to return true
	*	@param fPortalWidth is the portals' full width
	* 
	*/
	bool CanPortalSpawn(float fLinecastLength, FName sTag, float fPortalWidth, float fPortalHeight);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (AllowPrivateAccess = true))
		float lineCastLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (AllowPrivateAccess = true))
		float portalWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (AllowPrivateAccess = true))
		float portalHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (AllowPrivateAccess = true))
		FName climbableTag;

	UPROPERTY(BlueprintReadOnly, Category = "Portal", meta = (AllowPrivateAccess = true))
		bool bCanPortalSpawn;
#pragma endregion

public:
	APortalGameMode* asGameMode;

#pragma region Weapon
protected:
	int32 myWeaponType;
	int32 ammoAmount;
public:
	void RequestGunFromServer(int32 WeaponTypePayload, APortal_GE_IICharacter* charRef);
	void SetWeaponType(int32 payload) { myWeaponType = payload; }
	void SetCanShoot(bool payload) { bCanShoot = payload; }
	void SetAmmoAmount(int32 ammoAmountPayload) { ammoAmount = ammoAmountPayload; }
protected:
	UFUNCTION( BlueprintImplementableEvent )
		void ChangeGunColor(FLinearColor colorToChangeTo);

protected:
	/*
* weapon type:
*	0-> Assault Rifle
*	1-> Shotgun
*	2-> Rocket Launcher
*	3-> Portal Gun
*/
	UFUNCTION( Server, Reliable )
	void GivePlayerAGun(int32 weaponTypePayload, APortal_GE_IICharacter* charRef);
#pragma endregion
};


