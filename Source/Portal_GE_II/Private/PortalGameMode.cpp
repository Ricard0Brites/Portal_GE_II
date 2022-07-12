// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGameMode.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

APortalGameMode::APortalGameMode()
{
	 //set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Character/FirstPersonCharacter"));

	if(PlayerPawnClassFinder.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
	
}

void APortalGameMode::Respawn(AController* Controller)
{
	if(Controller)
	{
		if(HasAuthority())
		{
			FVector SpawnLocation1 = FVector(-1463.0f, -456.0f, 342.0f);
			FVector SpawnLocation2 = FVector(-1445.0f, 843.0f, 342.0f);
			FVector SpawnLocation3 = FVector(133.0f, -1345.0f, 342.0f);
			FVector SpawnLocation4 = FVector(448.0f, 261.0f, 342.0f);

			TArray<FVector> LocationArray;
			LocationArray.Add(SpawnLocation1);
			LocationArray.Add(SpawnLocation2);
			LocationArray.Add(SpawnLocation3);
			LocationArray.Add(SpawnLocation4);

			
			int i = rand() % 3;
			
			if(APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass,LocationArray[i], FRotator::ZeroRotator))
			{
				Controller->Possess(Pawn);
			}
		}
	}

}