// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/MovementComponent.h"
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

#pragma region Components
		/** Pawn mesh: 1st person view (arms; seen only by self) */
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* Mesh1P;

	/** Pawn mesh: 3rd person view (full mesh ) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* Mesh3P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* FP_Gun;

	/** Gun mesh: 3st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* FP_Gun3P;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;
#pragma endregion


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

	

#pragma endregion

public:
#pragma region DefaultFunctions
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
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

#pragma region PortalSpawnRotation
public:
	FRotator portalSpawnRotation;
#pragma endregion

#pragma region HealthSystem
protected:
	/** The player's maximum health. This is the highest that their health can be, and the value that their health starts at when spawned.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** The player's current health. When reduced to 0, they are considered dead.*/
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	/** RepNotify for changes made to current health.*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

public:

	/** Getter for Max Health.*/
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; } 

	/** Getter for Current Health.*/
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage( float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser ) override;


#pragma endregion
	
public:

	UPROPERTY(BlueprintReadWrite)
	UMovementComponent* movementComp;



};


