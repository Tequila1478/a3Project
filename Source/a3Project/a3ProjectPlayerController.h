// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h" // Replace CoreMinimal.h with EngineMinimal.h for a lighter include
#include "GameFramework/PlayerController.h"
#include "a3ProjectPlayerController.generated.h"

class UInputMappingContext;

/**
*
*/
UCLASS()
class A3PROJECT_API Aa3ProjectPlayerController : public APlayerController
{
GENERATED_BODY()

protected:

/** Input Mapping Context to be used for player input */
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
UInputMappingContext* InputMappingContext;

UFUNCTION(Server, Reliable, BlueprintCallable)
void Server_StartGame(const FString& MapName);

// Begin Actor interface
protected:

virtual void BeginPlay() override;

// End Actor interface
};
