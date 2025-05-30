// Copyright Epic Games, Inc. All Rights Reserved.


#include "a3ProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

void Aa3ProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn possessed: %s"), *GetPawn()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No pawn possessed!"));
	}


}

void Aa3ProjectPlayerController::Server_StartGame_Implementation(const FString& MapName)
{
	if (HasAuthority())
	{
		FString TravelCommand = FString::Printf(TEXT("%s?listen"), *MapName);
		GetWorld()->ServerTravel(TravelCommand);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Server_StartGame Executed"));
	}
}