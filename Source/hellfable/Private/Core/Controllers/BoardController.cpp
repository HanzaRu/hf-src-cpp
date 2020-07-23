#include "Core/Controllers/BoardController.h"
//Game
#include "AI/HFABoard.h"
#include "Core/HFAGamePawn.h"

UBoardController::UBoardController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBoardController::BeginPlay()
{
	Super::BeginPlay();
	_board = NewObject<UHFABoard>(this,BoardClass,"Board");
}

UHFABoard* UBoardController::GetBoard()
{
	return _board;
}

/*void UBoardController::OrderAttack(AHFAGamePawn* attacker, AHFAGamePawn* target)
{
	auto*& targeted = _orders.FindOrAdd(attacker);
	targeted = target;
	auto*& targetOrder = _orders.FindOrAdd(target);
}*/

/*void UBoardController::Recalculate()
{
	static TMap<AHFAGamePawn*, AHFAGamePawn*> process;
	process.Reset();
	process.Append(_orders);

	static TMap<AHFAGamePawn*, AHFAGamePawn*> opponents;
	opponents.Reset();

	static TMap<AHFAGamePawn*, AHFAGamePawn*> singles;
	singles.Reset();

	static TMap<AHFAGamePawn*, AHFAGamePawn*> alreadyInRange;
	alreadyInRange.Reset();

	while (process.Num() > 0)
	{
		for (auto kv : process)
		{
			auto** torder = process.Find(kv.Value);
			if (torder != nullptr && *torder == kv.Key)
			{
				auto vk = TPair<AHFAGamePawn*, AHFAGamePawn*>(kv.Value, kv.Key);
				bool kAttack = IsInAttackRange(kv);
				bool vAttack = IsInAttackRange(vk);
				if (kAttack && vAttack)
				{
					alreadyInRange.Add(kv);
					alreadyInRange.Add(vk);
				}
				else if (kAttack)
				{
					alreadyInRange.Add(kv);
					singles.Add(vk);
				}
				else if (vAttack)
				{
					alreadyInRange.Add(vk);
					singles.Add(kv);
				}
				else
				{
					opponents.Add(kv);
				}
				process.Remove(kv.Key);
				process.Remove(kv.Value);
			}
			else
			{
				//default
				if (IsInAttackRange(kv))
				{
					alreadyInRange.Add(kv);
				}
				else
				{
					singles.Add(kv);
				}
				process.Remove(kv.Key);
			}
			break;
		}
	}

	//neightbor priority
	for (auto pair : opponents)
	{

	}
}*/

bool UBoardController::IsInAttackDistance(AHFAGamePawn* attacker, AHFAGamePawn* target) const
{
	FIntPoint kpos, vpos;
	if (!_board->FindPosition(attacker, kpos) || !_board->FindPosition(target, vpos)) return false;
	switch (attacker->GetAttackType())
	{
		case EAttackType::MELEE:
			return UHFABoard::DiagonalDistance(kpos, vpos) <= 1;
		case EAttackType::RANGED:
			return UHFABoard::DiagonalDistance(kpos, vpos) <= 4;
	}
	return false;
}

