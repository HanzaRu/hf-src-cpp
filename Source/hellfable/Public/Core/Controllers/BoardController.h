#pragma once
//Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//Game
#include "BoardController.generated.h"
//Weak
class AHFAGamePawn;
class UHFABoard;

USTRUCT()
struct FOrderAttack
{
	GENERATED_USTRUCT_BODY()
public:
	AHFAGamePawn* Attacker;
	AHFAGamePawn* target;
};

USTRUCT()
struct FPairedAttack
{
	GENERATED_USTRUCT_BODY()
public:
	AHFAGamePawn* OpponentA;
	AHFAGamePawn* OpponentB;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLFABLE_API UBoardController : public UActorComponent
{
	GENERATED_BODY()
public:
	UBoardController();

	void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	UHFABoard* GetBoard();

	/*UFUNCTION(BlueprintCallable)
	void OrderAttack(AHFAGamePawn* attacker, AHFAGamePawn* target);*/

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHFABoard> BoardClass;

	/*UFUNCTION(BlueprintCallable)
	void AddUnitToBoard(AActor* actor);

	UFUNCTION(BlueprintCallable)
	bool AttackTarget(AActor* actor);*/

	UFUNCTION(BlueprintPure)
	bool IsInAttackDistance(AHFAGamePawn* attacker, AHFAGamePawn* target) const;
private:
	UPROPERTY()
	UHFABoard* _board;

	TMap<AHFAGamePawn*, AHFAGamePawn*> _orders;
	TMap<AHFAGamePawn*, AHFAGamePawn*> _oppositeOrders;

	/** internal solve orders and paths */
	//void Recalculate();
};
