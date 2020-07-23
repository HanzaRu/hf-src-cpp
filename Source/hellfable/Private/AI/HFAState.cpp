#include "AI/HFAState.h"
//Engine
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
//Game
#include "Core/HFAGameMode.h"
#include "Core/HFAGamePawn.h"
#include "AI/HFAStateMachineComponent.h"

void UHFAState::Enter(AHFAGamePawn* gamePawn)
{
	GamePawn = gamePawn;
	StateMachine = GamePawn->HFAStateMachineComponent;
	OnEnter();
}

void UHFAState::Tick(AHFAGamePawn* gamePawn, float TimeInState)
{
	GamePawn = gamePawn;
	StateMachine = gamePawn->HFAStateMachineComponent;
	GameMode = AHFAGameMode::GetHFAGameMode(gamePawn);
	OnTick(TimeInState);
}

void UHFAState::Exit(AHFAGamePawn* gamePawn)
{
	GamePawn = gamePawn;
	StateMachine = GamePawn->HFAStateMachineComponent;
	OnExit();
}