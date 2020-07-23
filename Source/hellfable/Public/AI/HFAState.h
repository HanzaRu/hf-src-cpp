#pragma once
//Engine
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//Game
#include "HFAState.generated.h"
//Weak
class AHFAGamePawn;
class AHFAGameMode;
class UHFAStateMachineComponent;

/**
 * Finite State Machine of Fighters
 */
UCLASS(Blueprintable, BlueprintType)
class HELLFABLE_API UHFAState : public UObject
{
	GENERATED_BODY()
public:
	
	void Enter(AHFAGamePawn* gamepawn);
	void Tick(AHFAGamePawn* gamepawn, float TimeInState);
	void Exit(AHFAGamePawn* gamepawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTick(float TimeInState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnExit();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	AHFAGamePawn* GamePawn;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	AHFAGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	UHFAStateMachineComponent* StateMachine;
};
