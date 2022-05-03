// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Portal_GE_IICharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class APortal_GE_IICharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Mesh, meta	= (allowprivateaccess = true))
		USkeletalMeshComponent* Mesh1P;

	/** Pawn mesh: 3rd person view (full mesh ) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh3P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta	= ( allowprivateaccess = true))
		USkeletalMeshComponent* FP_Gun;

	/** Gun mesh: 3st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (allowprivateaccess = true))
		USkeletalMeshComponent* FP_Gun3P;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta	= (allowprivateaccess = true))
		USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

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
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class APortal_GE_IIProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

#pragma endregion

protected:
	
#pragma region Inputs
	/** Fires a projectile or a portal */
	void OnFireLeft();
	/** Fires a projectile or a portal */
	void OnFireRight();


	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
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
#pragma endregion

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	
#pragma region PortalCrosshairCheck

	/** checks if the player can spawn a portal
		@param fLinecastLength is the length the linecast should be
		@param sTag is the tag that the cast is searching for to return true
		@param fPortalWidth is the portals' full width
	*/

	bool CanPortalSpawn(float fLinecastLength, FName sTag, float fPortalWidth, float fPortalHeight);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (allowprivateaccess = true))
		float lineCastLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (allowprivateaccess = true))
		float portalWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (allowprivateaccess = true))
		float portalHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal", meta = (allowprivateaccess = true))
		FName climbableTag;

	UPROPERTY(BlueprintReadOnly, Category = "Portal", meta = (allowprivateaccess = true))
		bool bCanPortalSpawn;
#pragma endregion

};


