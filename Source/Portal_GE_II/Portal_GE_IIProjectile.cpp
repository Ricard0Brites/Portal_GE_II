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

	bReplicates = true;
}

void APortal_GE_IIProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	asGameState = Cast<APortalGameState>(gameStateRef);
	asPortalManager = Cast<APortalManager>(UGameplayStatics::GetActorOfClass(this, portalManagerBpRef));
	SetOwner(asGameState);
}

void APortal_GE_IIProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (Hit.GetActor()->ActorHasTag("Wall") && bCanPortalSpawn)
		{
			if (HasAuthority())
			{
				asPortalManager->SpawnPortalOnAllClients(Hit.Location, bPortalTypeToSpawn, asPortalManager, Hit);
			}
			else
			{
				//call a  run-on-server function that calls a multi cast that spawns the portal in every player
				SR_SpawnPortals(Hit.Location, bPortalTypeToSpawn, Hit);
			}
			bCanPortalSpawn = false;
		}
		Destroy();
	}
}

#pragma region BulletParameters

void APortal_GE_IIProjectile::SR_SpawnPortals_Implementation(FVector location, bool portalType, FHitResult hitResult)
{
	asPortalManager->SpawnPortalOnAllClients(location, portalType, asPortalManager, hitResult);
	UE_LOG(LogTemp, Error, TEXT("TEST"));
}

void APortal_GE_IIProjectile::SR_GetBulletParams_Implementation(int32 weaponTypeIndexPayload, APortalGameState* gameStatePayload, APortal_GE_IIProjectile* projectileRef)
{
	projectileRef->SetDamage(gameStatePayload->GetWeaponDamage(weaponTypeIndexPayload));
	projectileRef->ProjectileMovement->MaxSpeed = gameStatePayload->GetBulletSpeed(weaponTypeIndexPayload);
	projectileRef->ProjectileMovement->ProjectileGravityScale = gameStatePayload->GetBulletGravityScale(weaponTypeIndexPayload);
}

void APortal_GE_IIProjectile::SetBulletParameters(int32 weaponTypeIndexPayload)
{
	if (HasAuthority())
	{
		if (asGameState)
		{
			SetDamage(asGameState->GetWeaponDamage(weaponTypeIndexPayload));
			ProjectileMovement->MaxSpeed = asGameState->GetBulletSpeed(weaponTypeIndexPayload);
			ProjectileMovement->ProjectileGravityScale = asGameState->GetBulletGravityScale(weaponTypeIndexPayload);
		}
	}
	else
	{
		SR_GetBulletParams(weaponTypeIndexPayload, asGameState, this);
	}
}

#pragma endregion

#pragma region Replication
void APortal_GE_IIProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

#pragma endregion
