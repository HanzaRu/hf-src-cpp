#include "Core/HFAGameMode.h"
//Engine
#include "Components/CapsuleComponent.h"
//Game
#include "Core/HFAGamePawn.h"
#include "Core/Controllers/BoardController.h"

AHFAGameMode::AHFAGameMode()
{
	CurrentBattleState = EBattleState::UNINITIALIZED;
	BoardController = CreateDefaultSubobject<UBoardController>("BoardController");
}

AHFAGameMode* AHFAGameMode::GetHFAGameMode(const UObject* worldContextObject)
{
	return worldContextObject->GetWorld()->GetAuthGameMode<AHFAGameMode>();
}

void AHFAGameMode::UpdateStartMarkers()
{
	if (BoardClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AHFAGameMode::UpdateStartMarkers] BoardClass is null or empty."));
		return;
	}

	TArray<AActor*> FoundMarkers;
	TArray<AActor*> FoundBoards;

	TArray<UStaticMeshComponent*> FoundTiles;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHFAStartMarker::StaticClass(), FoundMarkers);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BoardClass, FoundBoards);

	if (FoundBoards.Num() < 0) return;
	FoundBoards[0]->GetComponents(FoundTiles);

	for (int i = FoundMarkers.Num() - 1; i >= 0; i--)
	{
		float _lesserDistance = 0;
		UStaticMeshComponent* _tileTo = nullptr;

		_tileTo = nullptr;
		for (int j = FoundTiles.Num() - 1; j >= 0; j--)
		{
			float _distance = (FoundMarkers[i]->GetActorLocation() - FoundTiles[j]->GetComponentTransform().GetLocation()).Size();

			if (_distance < _lesserDistance || _lesserDistance == 0)
			{
				_lesserDistance = FMath::Abs(_distance);
				_tileTo = FoundTiles[j];
			}
		}

		if (_tileTo != nullptr)
		{
			FVector pos = _tileTo->GetComponentTransform().GetLocation();
			AHFAStartMarker* marker = Cast<AHFAStartMarker>(FoundMarkers[i]);
			pos = FVector(pos.X, pos.Y, FoundMarkers[i]->GetActorLocation().Z);
			FoundMarkers[i]->SetActorLocation(pos);
			FoundTiles.Remove(_tileTo);
		}
	}
}

void AHFAGameMode::SetBattleState(EBattleState BattleState)
{
	if (CurrentBattleState == BattleState) return;
	CurrentBattleState = BattleState;
	if (BattleState == EBattleState::BATTLE)
	{
		InitializeBattle();
	}
	OnBattleStateChanged.Broadcast(BattleState);
}

void AHFAGameMode::InitializeBattle()
{
	TArray<AActor*> GamePawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHFAGamePawn::StaticClass(), GamePawns);
	for (auto* actor : GamePawns)
	{
		auto* pawn = Cast<AHFAGamePawn>(actor);
		if (!pawn->IsInBattle()) continue;
		switch (pawn->GetTeam())
		{
			case EGamePawnTeam::PLAYER: PlayerPawns.Add(pawn); break;
			case EGamePawnTeam::ENEMY: EnemyPawns.Add(pawn); break;
		}
	}
}

void AHFAGameMode::VerifyConditionToEndBattle()
{
	auto playerAlive = IsAnyPlayerPawnAlive();
	auto enemyAlive = IsAnyEnemyPawnAlive();
	if (playerAlive && enemyAlive) return;
	SetBattleState(playerAlive ? EBattleState::VICTORY : EBattleState::LOSE);
}

TArray<AHFAGamePawn*> AHFAGameMode::GetPawnsFromTargetTeam(EGamePawnTeam team)
{
	return team == EGamePawnTeam::PLAYER ? PlayerPawns : EnemyPawns;
}

AHFAGamePawn* AHFAGameMode::GetMostDistantEnemy(AHFAGamePawn* pawn)
{
	AHFAGamePawn* target = nullptr;
	float maximumDistanceSquared = 0;

	auto& opposite = pawn->GetTeam() == EGamePawnTeam::PLAYER ? EnemyPawns : PlayerPawns;
	for (auto opponent : opposite)
	{
		if (opponent->IsDead()) continue;
		float currentDist = FVector::DistSquared(opponent->GetFeetPosition(), pawn->GetFeetPosition());
		if (currentDist < maximumDistanceSquared) continue;
		maximumDistanceSquared = currentDist;
		target = opponent;
	}

	return target;
}

AHFAGamePawn* AHFAGameMode::QueryTargetFromDirection(AHFAGamePawn* attacker)
{
	if (attacker == nullptr) return nullptr;
	auto& opposite = attacker->GetTeam() == EGamePawnTeam::PLAYER ? EnemyPawns : PlayerPawns;
	AHFAGamePawn* target = nullptr;
	float maximumDistanceSquared = 0;
	for (auto opponent : opposite)
	{
		if (opponent->IsDead()) continue;
		auto distanceSquared = FVector::DistSquared(attacker->GetFeetPosition(), opponent->GetFeetPosition());
		if (distanceSquared < maximumDistanceSquared) continue;
		maximumDistanceSquared = distanceSquared;
	}
	float maximumDotInvDist = -FLT_MAX;
	for (auto opponent : opposite)
	{
		if (opponent->IsDead()) continue;
		auto distanceSquared = FVector::DistSquared(attacker->GetFeetPosition(), opponent->GetFeetPosition());
		auto dot = attacker->GetDotProductTo(opponent);
		auto invdist = 1 - distanceSquared / maximumDistanceSquared;
		auto dotInvDist = dot * invdist;
		if (dotInvDist < maximumDotInvDist) continue;
		target = opponent;
		maximumDotInvDist = dotInvDist;
	}
	return target;
}

void AHFAGameMode::SetSpecialTimeDilation(AHFAGamePawn* specialOwner, bool begin)
{
	for (auto pawn : PlayerPawns)
	{
		pawn->CustomTimeDilation = (begin && pawn != specialOwner ? TimeDilationSpecialValue : 1);
	}
	for (auto enemy : EnemyPawns)
	{
		enemy->CustomTimeDilation = (begin ? TimeDilationSpecialValue : 1);
	}
}

void AHFAGameMode::OnSpecialTriggered(AHFAGamePawn* SpecialCaster)
{
	OnSpecialCasted.Broadcast(SpecialCaster);
}

void AHFAGameMode::OnSpecialEndedTriggered(AHFAGamePawn* SpecialCaster)
{
	OnSpecialEnded.Broadcast();
}

bool AHFAGameMode::IsAnyPlayerPawnAlive()
{
	for (auto* pawn : PlayerPawns)
	{
		if (pawn->IsAlive()) return true;
	}
	return false;
}

bool AHFAGameMode::IsAnyEnemyPawnAlive()
{
	for (auto* pawn : EnemyPawns)
	{
		if (pawn->IsAlive()) return true;
	}
	return false;
}