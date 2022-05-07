// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalClass.h"

// Sets default values
APortalClass::APortalClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));


	//creates the outer ring static mesh component (used as the colored ring)
	portalOuterRing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outer Ring Mesh"));
	portalOuterRing->SetupAttachment(RootComponent);
	portalOuterRing->SetRelativeScale3D(FVector(70.0f, 7.0f, 70.0f));
	portalOuterRing->SetRelativeRotation(FRotator(-90.0f,0.0f,180.0f));


	//creates the inside ring static mesh component (used as the actual portal itself)
	portalInnerRing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inner Ring Mesh"));
	portalInnerRing->SetupAttachment(RootComponent);
	portalInnerRing->SetRelativeScale3D(FVector(70.0f,70.0f,70.0f));
	portalInnerRing->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	portalInnerRing->SetRelativeLocation(FVector(-3.0f, 0.0f, 0.0f));

	//creates a scene capture component
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Component"));
	sceneCapture->SetupAttachment(RootComponent);
	sceneCapture->SetRelativeLocation(FVector(-20.0f, 0.0f,0.0f));
}

// Called when the game starts or when spawned
void APortalClass::BeginPlay()
{
	Super::BeginPlay();
	
}

