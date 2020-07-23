#pragma once
//Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
//Game
#include "HFAStartMarker.h"
#include "Core/HFAGamePawn.h"
#include "HFAGameMode.generated.h"
//Weak
class UBoardController;

UENUM(BlueprintType)
enum class EBattleState : uint8
{
	UNINITIALIZED = 0,
	PRE_BATTLE,
	BATTLE,
	VICTORY,
	LOSE
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleStateChanged, EBattleState, battlestate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecialCasted, AHFAGamePawn*, specialCaster);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpecialEnded);

/**
 * Default Game Mode of Hellfable
 */
UCLASS()
class HELLFABLE_API AHFAGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	/** Default Constructor */
	AHFAGameMode();

	/** Returns the current HFAGameMode. Returns a valid pointer on server. Returns nullptr on client.*/
	UFUNCTION(BlueprintPure, meta = (WorldContext = "worldContextObject"))
	static AHFAGameMode* GetHFAGameMode(const UObject* worldContextObject);

	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	void UpdateStartMarkers();

	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	void SetBattleState(EBattleState BattleState);

	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	void VerifyConditionToEndBattle();

	/** get all pawns from a team. */
	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	TArray<AHFAGamePawn*> GetPawnsFromTargetTeam(EGamePawnTeam team);

	/** gets most distant enemy  */
	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	AHFAGamePawn* GetMostDistantEnemy(AHFAGamePawn* pawn);

	/** queries a target with highest dot between current targets or returns nullptr. */
	UFUNCTION(BlueprintPure)
	AHFAGamePawn* QueryTargetFromDirection(AHFAGamePawn* attacker);

	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	void SetSpecialTimeDilation(AHFAGamePawn* specialOwner, bool begin);

	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	void OnSpecialTriggered(AHFAGamePawn* SpecialCaster);

	UFUNCTION(BlueprintCallable, Category = "HFAGameMode")
	void OnSpecialEndedTriggered(AHFAGamePawn* SpecialCaster);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> BoardClass;

	UPROPERTY(BlueprintAssignable)
	FOnBattleStateChanged OnBattleStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSpecialCasted OnSpecialCasted;

	UPROPERTY(BlueprintAssignable)
	FOnSpecialEnded OnSpecialEnded;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EBattleState CurrentBattleState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float NavTargetPositionThreshold = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BATTLE")
	float TimeDilationSpecialValue = 0.1f;
#pragma region COMPONENTS
	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly)
	UBoardController* BoardController;
#pragma endregion
private:
	TArray<AHFAGamePawn*> PlayerPawns;
	TArray<AHFAGamePawn*> EnemyPawns;

	void InitializeBattle();

	bool IsAnyPlayerPawnAlive();
	bool IsAnyEnemyPawnAlive();
};
