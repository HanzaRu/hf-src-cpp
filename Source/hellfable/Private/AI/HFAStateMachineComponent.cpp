#include "AI/HFAStateMachineComponent.h"
//Game
#include "Core/HFAGamePawn.h"
#include "Core/HFAGameMode.h"
#include "AI/HFAState.h"

UHFAStateMachineComponent::UHFAStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	GameMode = nullptr;
	GamePawn = nullptr;
	IsEnabled = true;
}

void UHFAStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AHFAGameMode::GetHFAGameMode(this);
	GamePawn = Cast<AHFAGamePawn>(GetOwner());
	CurrentStateTimestamp = GetWorld()->GetTimeSeconds();
	LastState = CurrentState;

	//build state machine
	for (auto state : StateMachine)
	{
		StateMachineInternal.Add(state.Key, NewObject<UHFAState>(this, state.Value));
	}

	//start the first state:
	auto* state = GetCurrentFSMState();
	if (state == nullptr) return;
	state->Enter(GamePawn);
}

void UHFAStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsEnabled) return;
	if (!GamePawn->IsInBattle()) return;
	if (GameMode->CurrentBattleState != EBattleState::BATTLE) return;
	//tick current state
	auto* state = GetCurrentFSMState();
	if (state == nullptr) return;
	state->Tick(GamePawn, GetWorld()->GetTimeSeconds() - CurrentStateTimestamp);
}

void UHFAStateMachineComponent::ChangeState(FName nextState)
{
	if (!IsEnabled) return;
	if (CurrentState == nextState) return;
	auto* currState = GetCurrentFSMState();
	if (currState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("failed to get current state"));
		return;
	}
	currState->Exit(GamePawn);
	LastState = CurrentState;
	CurrentState = nextState;
	CurrentStateTimestamp = GetWorld()->GetTimeSeconds();
	auto* nexState = GetCurrentFSMState();
	if (nexState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("failed to get next state"));
		return;
	}
	nexState->Enter(GamePawn);
}

void UHFAStateMachineComponent::SaveState(FName saveState)
{
	SavedState = saveState;
}

bool UHFAStateMachineComponent::CurrentStateIs(FName state)
{
	return state == CurrentState;
}

bool UHFAStateMachineComponent::LastStateIs(FName state)
{
	return state == LastState;
}

bool UHFAStateMachineComponent::SavedStateIs(FName state)
{
	return state == SavedState;
}

bool UHFAStateMachineComponent::CurrentStateIsAny(const TArray<FName>& states)
{
	return states.Contains(CurrentState);
}

bool UHFAStateMachineComponent::LastStateIsAny(const TArray<FName>& states)
{
	return states.Contains(LastState);
}

UHFAState* UHFAStateMachineComponent::GetCurrentFSMState()
{
	auto* state = StateMachineInternal.Find(CurrentState);
	if (state != nullptr)
	{
		return *state;
	}
	return nullptr;
}

void UHFAStateMachineComponent::SaveTarget(AHFAGamePawn* saveTarget)
{
	SavedTarget = saveTarget;
}

void UHFAStateMachineComponent::SetEnabled(bool isEnabled)
{
	IsEnabled = isEnabled;
}

