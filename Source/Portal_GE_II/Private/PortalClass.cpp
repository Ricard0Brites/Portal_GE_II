// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalClass.h"

// Sets default values
APortalClass::APortalClass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	this->SetActorTickEnabled(false);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));


	//creates the outer ring static mesh component (used as the colored ring)
	portalOuterRing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outer Ring Mesh"));
	portalOuterRing->SetupAttachment(RootComponent);
	portalOuterRing->SetRelativeScale3D(FVector(70, 70, 70));
	portalOuterRing->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0));

	//creates the inside ring static mesh component (used as the actual portal itself)
	portalInnerRing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inner Ring Mesh"));
	portalInnerRing->SetupAttachment(RootComponent);
	portalInnerRing->SetRelativeScale3D(FVector(70,70,70));
	portalInnerRing->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	portalInnerRing->SetRelativeLocation(FVector(-3.0f, 0.0f, 0.0f));

	//creates a scene capture component
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component"));
	sceneCapture->SetupAttachment(RootComponent);
	sceneCapture->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));

	//create capsule component
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	capsuleComponent->SetupAttachment(RootComponent);
	capsuleComponent->SetRelativeLocation(FVector(-25, 0, 0));
	capsuleComponent->SetCapsuleHalfHeight(130);
	capsuleComponent->SetCapsuleRadius(70);
}

// Called when the game starts or when spawned
void APortalClass::BeginPlay()
{
	Super::BeginPlay();
#pragma region CollisionDelegates
	//Component begin overlap
	portalOuterRing->OnComponentBeginOverlap.AddDynamic(this, &APortalClass::BeginOverlap);
	//component end overlap
	capsuleComponent->OnComponentEndOverlap.AddDynamic(this, &APortalClass::EndOverlap);
#pragma endregion

	if (GetWorld())
	{
		asPlayerCharacter = Cast<APortal_GE_IICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		asPortalManager = Cast<APortalManager>(UGameplayStatics::GetActorOfClass(this, portalManagerBpRef));
	}
	//tick is only enabled once the begin play is complete
	this->SetActorTickEnabled(true);
}

void APortalClass::Tick(float deltaTime)
{
	if (myPortalType && asPortalManager)
	{
		asPortalManager->SetOrangePortalCameraRotation(asPortalManager->CalculateAngleBetweenCharacterAndPortal(this));
	}
	else
	{
		asPortalManager->SetBluePortalCameraRotation(asPortalManager->CalculateAngleBetweenCharacterAndPortal(this));
	}
}

#pragma region Teleportation
void APortalClass::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
	if (OtherActor == asPlayerCharacter && canTeleport) 
	{
		//sets the variable on the corresponding portal 
		asPortalManager->SetOtherCanTeleport(this, false);
		asPortalManager->TeleportCharacter(this);
	}
}

void APortalClass::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//allow the portal to teleport again
	canTeleport = true;
}

#pragma endregion
