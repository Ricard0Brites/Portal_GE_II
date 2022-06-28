// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform/WeaponPlatform.h"
class APortalGameMode;

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

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(RootComponent);
	boxCollider->SetRelativeScale3D(FVector(1,1,2));
	boxCollider->SetRelativeLocation(FVector(0,0,60));
	boxCollider->SetCollisionObjectType(ECC_WorldDynamic);

	SetReplicates(true);
	
}

// Called when the game starts or when spawned
void AWeaponPlatform::BeginPlay() 
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPlatform::OnBoxBeginOverlap);
	if (GetWorld())
	{
		asGameMode = Cast<APortalGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	if (asGameMode != nullptr)
	{
		ChangeGunMeshColor(asGameMode->GetWeaponColor(iObjectType));
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
	if (OtherActor->ActorHasTag("Character"))
	{
		APortal_GE_IICharacter* asCharacter = Cast<APortal_GE_IICharacter>(OtherActor);
		asCharacter->RequestGun(iObjectType, asCharacter);
	}
}

