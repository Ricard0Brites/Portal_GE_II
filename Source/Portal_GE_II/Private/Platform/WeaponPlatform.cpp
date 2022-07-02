// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform/WeaponPlatform.h"
class APortalGameState;

// Sets default values
AWeaponPlatform::AWeaponPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	platformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Static Mesh Component"));
	RootComponent = platformMesh;

	hologramMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	hologramMesh->SetupAttachment(RootComponent);
	hologramMesh->SetRelativeLocation(FVector(0,0,100));
	hologramMesh->SetCollisionProfileName(FName("NoCollision"));

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(RootComponent);
	boxCollider->SetRelativeScale3D(FVector(1,1,2));
	boxCollider->SetRelativeLocation(FVector(0,0,60));
	boxCollider->SetCollisionObjectType(ECC_WorldDynamic);

	bReplicates = true;
	
}

// Called when the game starts or when spawned
void AWeaponPlatform::BeginPlay() 
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPlatform::OnBoxBeginOverlap);
	if (GetWorld())
	{
		asGameState = Cast<APortalGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
	if (asGameState != nullptr)
	{
		ChangeGunMeshColor(asGameState->GetWeaponColor(iObjectType));
	}
}

// Called every frame
void AWeaponPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	hologramMesh->SetRelativeRotation(FRotator(0, hologramMesh->GetRelativeRotation().Yaw + fHologramRotationSpeed, 0));
}

void AWeaponPlatform::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APortal_GE_IICharacter* asCharacter = Cast<APortal_GE_IICharacter>(OtherActor);
	
	if (OtherActor->ActorHasTag("Character") && iObjectType != asCharacter->GetPortalGunIndex())
	{
		asCharacter->RequestGun(iObjectType, asCharacter);
		asCharacter->SetCharacterHasWeapon(true);

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

void AWeaponPlatform::SR_DisableHologram_Implementation()
{
	MC_DisableAllHolograms();
}

void AWeaponPlatform::SR_EnableHologram_Implementation(AWeaponPlatform* weaponPlatformRefPayload)
{
	MC_EnableAllHolograms(weaponPlatformRefPayload);
}

void AWeaponPlatform::MC_DisableAllHolograms_Implementation()
{
	DisableHologramComponent();
}

void AWeaponPlatform::MC_EnableAllHolograms_Implementation(AWeaponPlatform* weaponPlatformRefPayload)
{
	APortalGameState* asGameStateLocal = Cast<APortalGameState>(UGameplayStatics::GetGameState(GetWorld()));
	weaponPlatformRefPayload->StartTimer(asGameStateLocal->GetPlatformCooldown(iObjectType));
}

void AWeaponPlatform::StartTimer(float timeInSeconds)
{
	FTimerHandle myHandle;
	GetWorldTimerManager().SetTimer(myHandle, this, &AWeaponPlatform::EnableHologramComponent, timeInSeconds, false);
}
