// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HFAGamePawnWidgetComponent.h"

UHFAGamePawnWidgetComponent::UHFAGamePawnWidgetComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UHFAGamePawnWidgetComponent::BeginPlay()
{
	AHFAGamePawn *autoPawn = Cast<AHFAGamePawn>(GetOwner());
	
	Super::BeginPlay();

	UHFACharacterWorldUI* autoWidget = Cast<UHFACharacterWorldUI>(GetUserWidgetObject());
	
	if (autoPawn != nullptr && autoWidget != nullptr)
	{
		autoWidget->Parent = autoPawn;
	}
}