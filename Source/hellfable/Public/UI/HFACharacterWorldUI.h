// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "Components/ProgressBar.h"
#include "HFACharacterWorldUI.generated.h"


/**
 * 
 */

class AHFAGamePawn;

UCLASS()
class HELLFABLE_API UHFACharacterWorldUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHFACharacterWorldUI(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UProgressBar* ProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	AHFAGamePawn* Parent;
};
