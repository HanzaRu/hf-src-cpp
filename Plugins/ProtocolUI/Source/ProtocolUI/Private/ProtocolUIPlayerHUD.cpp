#include "ProtocolUIPlayerHUD.h"
//Game
#include "ProtocolUIWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogProtocolUI);

AProtocolUIPlayerHUD::AProtocolUIPlayerHUD()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

AProtocolUIPlayerHUD* AProtocolUIPlayerHUD::GetProtocolUIPlayerHUD(const UObject* worldContextObject, int index)
{
	return UGameplayStatics::GetPlayerController(worldContextObject, index)->GetHUD<AProtocolUIPlayerHUD>();
}

void AProtocolUIPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	for (auto pair : StateConfigs)
	{
		auto st = GetState(pair.Key);
		st->Layer = pair.Value;
	}
#if WITH_EDITOR
	for (auto& state : EditorActiveStates)
	{
		SetState(state, true);
	}
#else
	for (auto& state : ActiveStates)
	{
		SetState(state, true);
	}
#endif
}

void AProtocolUIPlayerHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isRebuilt) return;
	ProcessStates();
	isRebuilt = true;
}

void AProtocolUIPlayerHUD::SetState(FName state, bool enabled)
{
	auto* st = GetState(state);
	if (enabled)
	{
		for (auto pair : States)
		{
			auto* sti = pair.Value;
			sti->IsActive &= sti->Layer != st->Layer;
		}
	}
	st->IsActive = enabled;
	isRebuilt = false;
}

UProtocolUIState* AProtocolUIPlayerHUD::GetState(FName state)
{
	auto** st = States.Find(state);
	if (st != nullptr)
	{
		return *st;
	}
	return States.Add(state, NewObject<UProtocolUIState>(this));
}

void AProtocolUIPlayerHUD::SetupWidget(TArray<TSubclassOf<UProtocolUIWidget>> widgets, int zOrder)
{
	auto playerController = GetOwningPlayerController();
	for (auto widget : widgets)
	{
		auto* ws = CreateWidget<UProtocolUIWidget>(playerController, widget);
		StateWidgets.Add(ws);
		ws->AddToViewport(zOrder);
		ProcessState(ws);
	}
}

void AProtocolUIPlayerHUD::ProcessStates()
{
	for (auto* widget : StateWidgets)
	{
		ProcessState(widget);
	}
}

void AProtocolUIPlayerHUD::ProcessState(UProtocolUIWidget* widget)
{
	for (auto state : widget->VisibleStates)
	{
		if (!GetState(state)->IsActive) continue;
		widget->SetWidgetVisibility(true);
		return;
	}
	widget->SetWidgetVisibility(false);
}
