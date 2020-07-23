// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "UI/HFACharacterWorldUI.h"
#include "Core/HFAGamePawn.h"
#include "HFAGamePawnWidgetComponent.generated.h"

/**
 * 
 */

UCLASS()
class HELLFABLE_API UHFAGamePawnWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHFAGamePawnWidgetComponent(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
};
