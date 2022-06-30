// Copyright Epic Games, Inc. All Rights Reserved.

#include "Portal_GE_IICharacter.h"
#include "Portal_GE_IIProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "PortalManager.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

#pragma region Default
APortal_GE_IICharacter::APortal_GE_IICharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	RootComponent = GetCapsuleComponent();
	RootComponent->SetRelativeRotation(FRotator(0, 0, 0));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a mesh component that will be used when being viewed from a '3rd person' view (when viewing this pawn)
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
	Mesh3P->SetOwnerNoSee(true);
	Mesh3P->SetupAttachment(RootComponent);
	Mesh3P->SetRelativeLocation(FVector(0, 0, -90));
	Mesh3P->SetRelativeRotation(FRotator(0, -90, 0));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	//FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));

	// Create a gun mesh component
	FP_Gun3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun3Person"));
	FP_Gun3P->SetOwnerNoSee(true);
	FP_Gun3P->SetupAttachment(Mesh3P);
	FP_Gun3P->AttachToComponent(Mesh3P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
	FP_Gun3P->SetRelativeLocation(FVector(-11, 6, -2));
	FP_Gun3P->SetRelativeRotation(FRotator(0, 100, 0));


	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	bReplicates = true;
}

void APortal_GE_IICharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (GetWorld())
	{
		asGameState = Cast<APortalGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}

	bCanShoot = false;
	bCanShootPortal = false;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	Mesh1P->SetHiddenInGame(false, true);
	
}

void APortal_GE_IICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bCanPortalSpawn = CanPortalSpawn(lineCastLength, acceptableTag, portalWidth, portalHeight);

	FString tempString = bCanShoot ? "can shoot: true" :  "can shoot: false";
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, TEXT(""));

}
#pragma endregion

bool APortal_GE_IICharacter::CanPortalSpawn(float fLinecastLength, FName sTag, float fPortalWidth, float fPortalHeight)
{
	if (GetWorld())
	{
		
		FVector LineCastEndLocation = GetControlRotation().Vector();
		FVector LineCastStartLocation = FP_Gun->GetComponentLocation();

		// Line Trace to determine if the object the player is looking at is a wall that accepts portals
		FHitResult result;
		bool bWasLineCastSuccessful = GetWorld()->LineTraceSingleByChannel(result, LineCastStartLocation, (LineCastEndLocation * fLinecastLength) + LineCastStartLocation, ECC_Visibility);
		if (bWasLineCastSuccessful)
		{
			if (result.GetActor()->ActorHasTag(sTag) == true)
			{
				FHitResult hitResult;
				//changes the X for the Y to create a new normal vector orthogonal to the wall
				FVector vNewNormal = FVector(result.Normal.Y, result.Normal.X, result.Normal.Z);
				FVector vZnormal = FVector(0, 0, 1);

				//horizontal left line trace
				bool bWasHorizontalLinecastNegativeSpaceSuccessful = GetWorld()->LineTraceSingleByChannel(
					hitResult,
					result.Location + result.Normal,
					((result.Location + result.Normal) - (vNewNormal * (fPortalWidth / 2))),
					ECC_Visibility);

				//horizontal right line trace
				bool bWasHorizontalLinecastPositiveSpaceSuccessful = GetWorld()->LineTraceSingleByChannel(
					hitResult,
					result.Location + result.Normal,
					((vNewNormal * (fPortalWidth / 2)) + result.Location + result.Normal),
					ECC_Visibility);

				//Vertical top line Trace
				bool bWasVerticalLinecastPositiveSpaceSuccessful = GetWorld()->LineTraceSingleByChannel(
					hitResult,
					result.Location + result.Normal,
					((vZnormal * fPortalHeight) + (result.Location + result.Normal)),
					ECC_Visibility);

				//Vertical Bottom Line Trace
				bool bWasVerticalLineCastNegativeSpaceSuccessful = GetWorld()->LineTraceSingleByChannel(
					hitResult,
					result.Location + result.Normal,
					((result.Location + result.Normal) - (vZnormal * fPortalHeight)),
					ECC_Visibility);

				if (!bWasHorizontalLinecastNegativeSpaceSuccessful && !bWasHorizontalLinecastPositiveSpaceSuccessful && !bWasVerticalLineCastNegativeSpaceSuccessful && !bWasVerticalLinecastPositiveSpaceSuccessful)
				{
					return true;
				}
			}
		}
	}
	return false;
}

#pragma region Input

void APortal_GE_IICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APortal_GE_IICharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APortal_GE_IICharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APortal_GE_IICharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APortal_GE_IICharacter::LookUpAtRate);

	// Bind fire event
	PlayerInputComponent->BindAction("FireLeft", IE_Pressed, this, &APortal_GE_IICharacter::OnFireLeft);
	PlayerInputComponent->BindAction("FireRight", IE_Pressed, this, &APortal_GE_IICharacter::OnFireRight);
}

void APortal_GE_IICharacter::OnFireLeft()
{
	if (bCanShoot)
	{
		// try and fire a projectile
		if (ProjectileClass[iWeaponType] != nullptr)
		{
			if (GetWorld() != nullptr)
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				if(HasAuthority())
				{
					// spawn the projectile at the muzzle
					spawnedProjectileLMB = GetWorld()->SpawnActor<APortal_GE_IIProjectile>(ProjectileClass[iWeaponType], SpawnLocation, SpawnRotation);

					//updates the variable value
					//updates the projectile values too
					OnRep_SpawnedProjectileLMB();
					//decrement ammo
					if (iAmmoAmount > 0)
					{
						HasAuthority() ? iAmmoAmount -= 1 : SR_PlayerShotBullet(this);
					}
				}
				else
				{
					SR_SpawnBullet(spawnedProjectileLMB, ProjectileClass[iWeaponType], SpawnLocation, SpawnRotation, this);
				}
			}
		}

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void APortal_GE_IICharacter::OnFireRight()
{
	if (bCanShootPortal)
	{
		// try and fire a projectile
		if (ProjectileClass[iWeaponType] != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				//get the prespective rotation (where the player is looking towards)
				const FRotator SpawnRotation = GetControlRotation();

				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				// spawn the projectile at the muzzle
				if (HasAuthority())
				{
					//spawn bullet on server
					spawnedProjectileRMB = World->SpawnActor<APortal_GE_IIProjectile>(ProjectileClass[iWeaponType], SpawnLocation, SpawnRotation);
				}
				else
				{
					//spawn bullet on client
					SR_SpawnPortalBullet(spawnedProjectileRMB, ProjectileClass[iWeaponType], SpawnLocation, SpawnRotation, this);

				}
			}
		}

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void APortal_GE_IICharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APortal_GE_IICharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APortal_GE_IICharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APortal_GE_IICharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

#pragma endregion

#pragma region Server
//triggers both local and server side gun spawn and property assignment
void APortal_GE_IICharacter::RequestGun(int32 WeaponTypePayload, APortal_GE_IICharacter* charRef)
{
	//if its not the portal gun
	if (WeaponTypePayload == iPortalGunIndex)
	{
		//this is the portal Gun
		if (HasAuthority())
		{
			//activate player can shoot
			charRef->SetCanShoot(true);

			//update the variable in client
			OnRep_UpdateCanShoot();

			//allows the gun to shoot portals
			charRef->SetCanShootPortals(true);

			//update the variable in client
			OnRep_UpdateCanShootPortal();

			//set weapon type
			charRef->SetWeaponType(WeaponTypePayload);

			//update the variable in client
			OnRep_UpdateWeaponType();

			//change gun color
			charRef->ChangeGunColor(asGameState->GetWeaponColor(WeaponTypePayload));
			//portal gun has no ammo
		}
		else
		{
			//rpc
			SR_GivePlayerAGun(WeaponTypePayload, charRef);
		}
	}
	else
	{
		if (HasAuthority())
		{
			//activate player can shoot
			charRef->SetCanShoot(true);
			//update the variable in client
			OnRep_UpdateCanShoot();

			//allows the gun to shoot portals
			charRef->SetCanShootPortals(false);
			//update the variable in client
			OnRep_UpdateCanShootPortal();

			//set weapon type
			charRef->SetWeaponType(WeaponTypePayload);
			//update the variable in client
			OnRep_UpdateWeaponType();

			//change gun color
			//value replicated in blueprint since this is not logic
			charRef->ChangeGunColor(asGameState->GetWeaponColor(WeaponTypePayload));

			//add ammo
			charRef->SetAmmoAmount(asGameState->GetWeaponAmmoAmount(WeaponTypePayload));
			OnRep_UpdateAmmoAmount();
		}
		else
		{
			//rpc
			SR_GivePlayerAGun(WeaponTypePayload, charRef);
		}
	}
}

void APortal_GE_IICharacter::SR_GivePlayerAGun_Implementation(int32 weaponTypePayload, APortal_GE_IICharacter* charRef)
{
	if (weaponTypePayload == iPortalGunIndex)
	{
		//this is the portal Gun
		
		//activate player can shoot
		charRef->SetCanShoot(true);

		//update the variable in client
		OnRep_UpdateCanShoot();

		//allows the gun to shoot portals
		charRef->SetCanShootPortals(true);

		//update the variable in client
		OnRep_UpdateCanShootPortal();

		//set weapon type
		charRef->SetWeaponType(weaponTypePayload);

		//update the variable in client
		OnRep_UpdateWeaponType();

		//change gun color
		charRef->ChangeGunColor(asGameState->GetWeaponColor(weaponTypePayload));
		//portal gun has no ammo
	}
	else
	{
		//activate player can shoot
		charRef->SetCanShoot(true);
		//update the variable in client
		OnRep_UpdateCanShoot();

		//allows the gun to shoot portals
		charRef->SetCanShootPortals(false);
		//update the variable in client
		OnRep_UpdateCanShootPortal();

		//set weapon type
		charRef->SetWeaponType(weaponTypePayload);
		//update the variable in client
		OnRep_UpdateWeaponType();

		//change gun color
		//value replicated in blueprint since this is not logic
		charRef->ChangeGunColor(asGameState->GetWeaponColor(weaponTypePayload));

		//add ammo
		charRef->SetAmmoAmount(asGameState->GetWeaponAmmoAmount(weaponTypePayload));
		OnRep_UpdateAmmoAmount();
	}
	
}

void APortal_GE_IICharacter::SR_PlayerShotBullet_Implementation(APortal_GE_IICharacter* charRef)
{
	charRef->SetAmmoAmount(charRef->GetAmmoAmount() - 1);
	charRef->OnRep_UpdateAmmoAmount();
}

void APortal_GE_IICharacter::SR_SetCanShoot_Implementation(APortal_GE_IICharacter* charRef, bool payload)
{
	charRef->SetCanShoot(payload);
	charRef->OnRep_UpdateCanShoot();
}

void APortal_GE_IICharacter::SR_SpawnBullet_Implementation(APortal_GE_IIProjectile* spawnedProjectilePayload, TSubclassOf<APortal_GE_IIProjectile> projectileClassPayload, FVector SpawnLocationPayload, FRotator SpawnRotationPayload, APortal_GE_IICharacter* characterReferencePayload)
{
	// spawn the projectile at the muzzle
	characterReferencePayload->SetSpawnedProjectileLMB(GetWorld()->SpawnActor<APortal_GE_IIProjectile>(projectileClassPayload, SpawnLocationPayload, SpawnRotationPayload));

	//update multiple values (see function)
	OnRep_SpawnedProjectileLMB();

	if (iAmmoAmount > 0)
	{
		HasAuthority() ? iAmmoAmount -= 1 : SR_PlayerShotBullet(this);
	}
}

void APortal_GE_IICharacter::SR_SpawnPortalBullet_Implementation(APortal_GE_IIProjectile* spawnedProjectilePayload, TSubclassOf<APortal_GE_IIProjectile> projectileSubclassPayload, FVector spawnLocationPayload, FRotator spawnRotationPayload, APortal_GE_IICharacter* charRefPayload)
{
	charRefPayload->SetSpawnedProjectileRMB(GetWorld()->SpawnActor<APortal_GE_IIProjectile>(projectileSubclassPayload, spawnLocationPayload, spawnRotationPayload));
	
	//update the value of the projectile reference
	//update the values of the projectile
	OnRep_SpawnedProjectileRMB();
}

void APortal_GE_IICharacter::RequestAmmo(int32 iWeaponTypePayload)
{
	SetAmmoAmount(asGameState->GetWeaponAmmoAmount(iWeaponTypePayload));
}

#pragma endregion

#pragma region Replication

void APortal_GE_IICharacter::OnRep_UpdateCanShoot()
{
	// Logs
	if (bCanShoot)
	{
		 UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::bCanShoot ----> True"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::bCanShoot ----> False"))
	}
}

void APortal_GE_IICharacter::OnRep_UpdateCanShootPortal()
{
	//logs
	if (bCanShootPortal)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::bCanShootPortal ----> True"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::bCanShootPortal ----> False"))
	}
}

void APortal_GE_IICharacter::OnRep_UpdateWeaponType()
{
	//logs
	
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::iWeaponType ----> %d "), iWeaponType);
}

void APortal_GE_IICharacter::OnRep_UpdateAmmoAmount()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::iAmmoAmount ----> %d "), iAmmoAmount);
}

void APortal_GE_IICharacter::OnRep_SpawnedProjectileLMB()
{
	//portal properties
	if (spawnedProjectileLMB != nullptr)
	{
		spawnedProjectileLMB->bCanPortalSpawn = false; //makes sure the portal isnt accidentally spawned
	}
	

	if (bCanShootPortal && spawnedProjectileLMB != nullptr)
	{
		bCanPortalSpawn ? spawnedProjectileLMB->bCanPortalSpawn = true : spawnedProjectileLMB->bCanPortalSpawn = false; // allows the portal to spawn
		spawnedProjectileLMB->bPortalTypeToSpawn = true; // Blue Portal
	}
	//normal weapon properties
	if (iWeaponType != iPortalGunIndex)
	{
		//set bullet properties
		if (spawnedProjectileLMB != nullptr)
		{
			spawnedProjectileLMB->SetBulletParameters(iWeaponType);
		}

		// <= 1 because this is verified by the bullet
		if (HasAuthority() && iAmmoAmount <= 1)
		{
			//set can shoot
			HasAuthority() ? bCanShoot = false : SR_SetCanShoot(this, false);
		}
		else if (!HasAuthority() && iAmmoAmount <= 0)
		{
			//set can shoot
			HasAuthority() ? bCanShoot = false : SR_SetCanShoot(this, false);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::spawnedProjectileLMB"));
}

void APortal_GE_IICharacter::OnRep_SpawnedProjectileRMB()
{
	// portal parameters
	bCanPortalSpawn ? spawnedProjectileRMB->bCanPortalSpawn = true : spawnedProjectileRMB->bCanPortalSpawn = false; // allows the portal to spawn
	spawnedProjectileRMB->bPortalTypeToSpawn = false; //Orange Portal
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Notify .............. APortal_GE_IICharacter::spawnedProjectileRMB"));
}

void APortal_GE_IICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APortal_GE_IICharacter, bCanShoot);
	DOREPLIFETIME(APortal_GE_IICharacter, bCanShootPortal);
	DOREPLIFETIME(APortal_GE_IICharacter, iWeaponType);
	DOREPLIFETIME(APortal_GE_IICharacter, iAmmoAmount);
	DOREPLIFETIME(APortal_GE_IICharacter, spawnedProjectileLMB);
	DOREPLIFETIME(APortal_GE_IICharacter, spawnedProjectileRMB);
}

#pragma endregion
