// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform/AmmoPlatform.h"

void AAmmoPlatform::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("test overlap"));
}
