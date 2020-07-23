#pragma once
//Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//Game
#include "HFAStateMachineComponent.generated.h"
//Weak
class UHFAState;
class AHFAGamePawn;
class AHFAGameMode;

UCLASS()
class HELLFABLE_API UHFAStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFAStateMachineComponent();

	void BeginPlay() override;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, TSubclassOf<UHFAState>> StateMachine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CurrentState;

	UFUNCTION(BlueprintCallable)
	void ChangeState(FName nextState);

	UFUNCTION(BlueprintCallable)
	void SaveState(FName saveState);

	UFUNCTION(BlueprintPure)
	bool CurrentStateIs(FName state);

	UFUNCTION(BlueprintPure)
	bool LastStateIs(FName state);

	UFUNCTION(BlueprintPure)
	bool SavedStateIs(FName state);

	UFUNCTION(BlueprintPure)
	bool CurrentStateIsAny(const TArray<FName>& states);

	UFUNCTION(BlueprintPure)
	bool LastStateIsAny(const TArray<FName>& states);

	UFUNCTION(BlueprintPure)
	UHFAState* GetCurrentFSMState();

	UFUNCTION(BlueprintCallable)
	void SaveTarget(AHFAGamePawn* saveTarget);

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool isEnabled);
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName, UHFAState*> StateMachineInternal;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHFAGamePawn* GamePawn;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHFAGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentStateTimestamp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName LastState;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName SavedState;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHFAGamePawn* SavedTarget;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsEnabled;
};
