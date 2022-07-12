// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/MovementComponent.h"
#include "Public/PortalGameState.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Portal_GE_IICharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class APortalGameState;
class UWorld;


UCLASS(config=Game)
class APortal_GE_IICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APortal_GE_IICharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

private:

#pragma region BulletSpawning
	UPROPERTY( ReplicatedUsing = OnRep_SpawnedProjectileLMB )
		APortal_GE_IIProjectile* spawnedProjectileLMB;
	UPROPERTY( ReplicatedUsing = OnRep_SpawnedProjectileLMB )
		APortal_GE_IIProjectile* spawnedProjectileRMB;

public:
	void SetSpawnedProjectileLMB(APortal_GE_IIProjectile* projectilePayload) { spawnedProjectileLMB = projectilePayload; }
	void SetSpawnedProjectileRMB(APortal_GE_IIProjectile* projectilePayload) { spawnedProjectileRMB = projectilePayload; }
#pragma endregion

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
		TArray<TSubclassOf<APortal_GE_IIProjectile>> ProjectileClass;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* Mesh3P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* FP_Gun;

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
	

	USkeletalMeshComponent* GetMesh1P() { return Mesh1P; }

	USceneComponent* GetRootComponent() { return RootComponent; }
	
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
		FName acceptableTag;

	UPROPERTY(BlueprintReadOnly, Category = "Portal", meta = (AllowPrivateAccess = true))
		bool bCanPortalSpawn;
#pragma endregion

#pragma region Weapon

	
	
	bool bHasWeapon;



public:
	void SetCharacterHasWeapon(bool payload) { bHasWeapon = payload; }
	bool GetCharacterHasWeapon() { return bHasWeapon; }

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

#pragma region Multiplayer	

private:
	/*
	* this value is 0 based
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapons", meta = (AllowPrivateAccess = true))
		int32 iPortalGunIndex;

	UPROPERTY( BlueprintReadOnly ,ReplicatedUsing = OnRep_UpdateCanShoot, meta = ( AllowPrivateAccess = true))
		bool bCanShoot;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateCanShootPortal, meta = (AllowPrivateAccess = true))
		bool bCanShootPortal;

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateWeaponType)
		int32 iWeaponType;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateAmmoAmount)
		int32 iAmmoAmount;

	UPROPERTY(EditAnywhere, Category = "Weapons")
		float RifleDamage;

	UPROPERTY(EditAnywhere, Category = "Weapons")
		float HeadShotMultiplier;

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeGunColor(FLinearColor colorToChangeTo);

	void Shoot(int32 iWeapon);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot(int32 iWeapon);
	bool ServerShoot_Validate(int32 iWeapon);
	void ServerShoot_Implementation(int32 iWeapon);

	void Die();
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	    void MultiDie();
	    bool MultiDie_Validate();
	    void MultiDie_Implementation();

FTimerHandle DestroyHandle;
void CallDestroy();

#pragma region ServerFunctions
	/*
* weapon type:
*	0-> Assault Rifle
*	1-> Shotgun
*	2-> Rocket Launcher
*	3-> Portal Gun
*/
public:
	UFUNCTION(Server, Reliable)
		void SR_GivePlayerAGun(int32 weaponTypePayload, APortal_GE_IICharacter* charRef);

	//decrements a bullet from the server side ammo count
	UFUNCTION(Server, Reliable)
		void SR_PlayerShotBullet(APortal_GE_IICharacter* charRef);

	UFUNCTION(Server, Reliable)
		void SR_SetCanShoot(APortal_GE_IICharacter* charRef, bool payload);
	UFUNCTION(Server, Reliable)
		void SR_SpawnBullet(
			APortal_GE_IIProjectile* spawnedProjectilePayload,
			TSubclassOf<APortal_GE_IIProjectile> projectileClassPayload,
			FVector SpawnLocationPayload,
			FRotator spawnRotationPayload,
			APortal_GE_IICharacter* characterReferencePayload
		);
	UFUNCTION(Server, Reliable)
		void SR_SpawnPortalBullet(
			APortal_GE_IIProjectile* spawnedProjectilePayload,
			TSubclassOf<APortal_GE_IIProjectile> projectileSubclassPayload,
			FVector spawnLocationPayload,
			FRotator spawnRotationPayload,
			APortal_GE_IICharacter* charRefPayload
		);
#pragma endregion


#pragma endregion
	
	void RequestAmmo(int32 iWeaponTypePayload);

	//game mode casting reference
	APortalGameState* asGameState;

#pragma region OnRep
	//Rep notify for each replicated variable
	UFUNCTION()
		void OnRep_UpdateCanShoot();

	UFUNCTION()
		void OnRep_UpdateCanShootPortal();

	UFUNCTION()
		void OnRep_UpdateWeaponType();

	UFUNCTION()
		void OnRep_UpdateAmmoAmount();

	UFUNCTION()
		void OnRep_SpawnedProjectileLMB();

	UFUNCTION()
		void OnRep_SpawnedProjectileRMB();
#pragma endregion

#pragma region Getters and Setters
	//requests a gun (triggers both locally and server side weapon spawn)
	void RequestGun(int32 WeaponTypePayload, APortal_GE_IICharacter* charRef);

	//set weapon parameters on spawn
	void SetWeaponType(int32 payload) { iWeaponType = payload; }

	void SetCanShoot(bool payload) { bCanShoot = payload; }

	void SetAmmoAmount(int32 ammoAmountPayload) { iAmmoAmount = ammoAmountPayload; OnRep_UpdateAmmoAmount(); }

	void SetCanShootPortals(bool payload) { bCanShootPortal = payload; }

	//get parameters
	int32 GetAmmoAmount() { return iAmmoAmount; }

	int32 GetWeaponType() { return iWeaponType; }

	// Variable replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


	int32 GetPortalGunIndex() { return iPortalGunIndex; }
#pragma endregion


};