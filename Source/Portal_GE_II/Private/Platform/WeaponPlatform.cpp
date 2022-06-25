// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform/WeaponPlatform.h"

// Sets default values
AWeaponPlatform::AWeaponPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	platformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Static Mesh Component"));
	RootComponent = platformMesh;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	weaponMesh->SetupAttachment(RootComponent);
	weaponMesh->SetRelativeLocation(FVector(0,0,100));
}

// Called when the game starts or when spawned
void AWeaponPlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	weaponMesh->SetRelativeRotation(FRotator(0, weaponMesh->GetRelativeRotation().Yaw + fWeaponRotationSpeed, 0));
}

