// Copyright Epic Games, Inc. All Rights Reserved.

#include "Portal_GE_IIProjectile.h"


APortal_GE_IIProjectile::APortal_GE_IIProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APortal_GE_IIProjectile::OnHit);// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 20000.f;
	ProjectileMovement->MaxSpeed = 20000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void APortal_GE_IIProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetOwner(gameStateRef);
	asGameState = Cast<APortalGameState>(gameStateRef);
	asPortalManager = Cast<APortalManager>(UGameplayStatics::GetActorOfClass(this, portalManagerBpRef));
}

void APortal_GE_IIProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (Hit.GetActor()->ActorHasTag("Wall") && bCanPortalSpawn)
		{
			//call a  run-on-server function that calls a multi cast that spawns the portal in every client
			SR_SpawnPortals(Hit.Location, bPortalTypeToSpawn, Hit);
			
			bCanPortalSpawn = false;
		}
		Destroy();
	}
}

void APortal_GE_IIProjectile::SR_SpawnPortals_Implementation(FVector location, bool portalType, FHitResult hitResult)
{
	if (HasAuthority())
	{
		asGameState->SpawnPortalOnAllClients(location, portalType, asPortalManager, hitResult);
	}
}

void APortal_GE_IIProjectile::SR_GetBulletParams_Implementation(int32 weaponTypeIndexPayload)
{
	/*SetDamage(asGameMode->GetWeaponDamage(weaponTypeIndexPayload));
	ProjectileMovement->InitialSpeed = asGameMode->GetBulletSpeed(weaponTypeIndexPayload);
	ProjectileMovement->ProjectileGravityScale = asGameMode->GetBulletGravityScale(weaponTypeIndexPayload);*/
}

void APortal_GE_IIProjectile::SetBulletParameters(int32 weaponTypeIndexPayload)
{
	if (HasAuthority())
	{
		/*if (asGameMode)
		{
			SetDamage(asGameMode->GetWeaponDamage(weaponTypeIndexPayload));
			ProjectileMovement->InitialSpeed = asGameMode->GetBulletSpeed(weaponTypeIndexPayload);
			ProjectileMovement->ProjectileGravityScale = asGameMode->GetBulletGravityScale(weaponTypeIndexPayload);
		}*/
	}
	else
	{
		SR_GetBulletParams(weaponTypeIndexPayload);
	}
}
