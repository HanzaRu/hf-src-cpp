#include "AI/HFABoard.h"
#include "GraphAStar.h"

const FIntPoint BoardNeightbors[8] = {
	FIntPoint(0, 1),
	FIntPoint(0, -1),
	FIntPoint(1, 0),
	FIntPoint(-1, 0),
	FIntPoint(-1, 1),
	FIntPoint(1, 1),
	FIntPoint(-1, -1),
	FIntPoint(1, 0)
};

struct FBoardPathFilter
{
	FBoardPathFilter(UHFABoard* board, AActor* actor)
	{
		_board = board;
		_actor = actor;
	}

	float GetHeuristicScale() const
	{
		return 1.0f;
	}

	float GetHeuristicCost(const UHFABoard::FNodeRef StartNodeRef, const UHFABoard::FNodeRef EndNodeRef) const
	{
		return UHFABoard::DiagonalDistance(StartNodeRef, EndNodeRef);
	}

	float GetTraversalCost(const UHFABoard::FNodeRef StartNodeRef, const UHFABoard::FNodeRef EndNodeRef) const
	{
		return UHFABoard::ManhattanDistance(StartNodeRef, EndNodeRef);
	}

	bool IsTraversalAllowed(const UHFABoard::FNodeRef NodeA, const UHFABoard::FNodeRef NodeB) const
	{
		return _board->IsOpen(NodeB) && _board->IsPawnOpen(_actor, NodeB);
	}

	bool WantsPartialSolution() const
	{
		return false;
	}
private:
	UHFABoard* _board;
	AActor* _actor;
};

UHFABoard::UHFABoard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	_size = 10;
	_nodeSize = 200;
}

void UHFABoard::PostInitProperties()
{
	Super::PostInitProperties();
	_uppertop = -FVector(1, 1, 0) * 0.5f * _nodeSize * _size;
	_origin = _uppertop + FVector(1, 1, 0) * 0.5f * _nodeSize;
}

void UHFABoard::AddPawn(AActor* actor)
{
	if (_pawns.Contains(actor)) return;
	_pawns.Add(actor);
}

void UHFABoard::RemovePawn(AActor* actor)
{
	if (!_pawns.Contains(actor)) return;
	_pawns.Remove(actor);
}

bool UHFABoard::QueryAdjacentPosition(AActor* actor, FIntPoint target, FIntPoint& out)
{
	out = FIntPoint::ZeroValue;

	if (actor == nullptr) return false;
	auto from = GetBoardPosition(actor->GetActorLocation());
	if (!IsValid(from) || !IsValid(target)) return false;

	float maximumDistance = FLT_MAX;
	bool found = false;
	for (int i = 0; i < GetNeighbourCount(target); i++)
	{
		auto node = GetNeighbour(target, i);
		if (IsNotValid(node)) continue;
		if (IsClosed(node)) continue;
		if (IsPawnClosed(actor, node)) continue;
		if (from == node)
		{
			out = from;
			return true;
		}
		auto distance = FVector2D::DistSquared(node, from);
		if (distance > maximumDistance) continue;
		maximumDistance = distance;
		out = node;
		found = true;
	}
	return found;
}

bool UHFABoard::QueryAdjacentInvertedPosition(AActor* actor, FIntPoint target, FIntPoint& out)
{
	out = FIntPoint::ZeroValue;

	if (actor == nullptr) return false;
	auto from = GetBoardPosition(actor->GetActorLocation());
	if (!IsValid(from) || !IsValid(target)) return false;

	float minimumDistance = FLT_MIN;
	bool found = false;
	for (int i = 0; i < GetNeighbourCount(target); i++)
	{
		auto node = GetNeighbour(target, i);
		if (IsNotValid(node)) continue;
		if (IsClosed(node)) continue;
		if (IsPawnClosed(actor, node)) continue;
		if (from == node)
		{
			out = from;
			return true;
		}
		auto distance = FVector2D::DistSquared(node, from);
		if (distance < minimumDistance) continue;
		minimumDistance = distance;
		out = node;
		found = true;
	}
	return found;
}

bool UHFABoard::FindPath(AActor* actor, FIntPoint end, TArray<FIntPoint>& navpoints)
{
	if (actor == nullptr) return false;
	UpdateClosedPawnList();
	FGraphAStar<UHFABoard> astar(*this);
	auto actorpos = GetBoardPosition(actor->GetActorLocation());
	return astar.FindPath(actorpos, end, FBoardPathFilter(this, actor), navpoints) == EGraphAStarResult::SearchSuccess;
}

bool UHFABoard::FindPathInWorld(AActor* actor, FIntPoint end, TArray<FVector>& navpoints)
{
	static TArray<FIntPoint> points;
	points.Reset();
	if (!FindPath(actor, end, points)) return false;
	for (auto point : points)
	{
		navpoints.Add(GetWorldPosition(point));
	}
	return true;
}

bool UHFABoard::FindPosition(AActor* actor, FIntPoint& out)
{
	out = GetBoardPosition(actor->GetActorLocation());
	return IsValid(out);
}

bool UHFABoard::IsValid(FIntPoint position) const
{
	return position.X >= 0 && position.X < _size
		&& position.Y >= 0 && position.Y < _size;
}

bool UHFABoard::IsNotValid(FIntPoint position) const
{
	return !IsValid(position);
}

bool UHFABoard::IsOpen(FIntPoint position) const
{
	return !IsClosed(position);
}

bool UHFABoard::IsClosed(FIntPoint position) const
{
	return _closed.Contains(position);
}

bool UHFABoard::IsPawnOpen(AActor* actor, FIntPoint position) const
{
	return !IsPawnClosed(actor, position);
}

bool UHFABoard::IsPawnClosed(AActor* actor, FIntPoint position) const
{
	auto close = _pawnsclosed.Find(position);
	if (close != nullptr)
	{
		return *close != actor;
	}
	return false;
}

FIntPoint UHFABoard::GetBoardPosition(FVector position)
{
	//normalized by uppertop (not center of uppertop cell);
	auto npos = position - _uppertop;
	return FIntPoint(FMath::TruncToInt(npos.X / _nodeSize), FMath::TruncToInt(npos.Y / _nodeSize));
}

void UHFABoard::UpdateClosedPawnList()
{
	_pawnsclosed.Reset();
	for (auto* pawn : _pawns)
	{
		auto& close = _pawnsclosed.FindOrAdd(GetBoardPosition(pawn->GetActorLocation()));
		close = pawn;
	}
}

FVector UHFABoard::GetWorldPosition(FIntPoint point)
{
	return _origin + _offset + FVector(point.X, point.Y, 0) * _nodeSize;
}

int UHFABoard::ManhattanDistance(FIntPoint a, FIntPoint b)
{
	return FMath::Abs(a.X - b.X) + FMath::Abs(a.Y - b.Y);
}

int UHFABoard::DiagonalDistance(FIntPoint a, FIntPoint b)
{
	return FMath::Max(FMath::Abs(a.X - b.X), FMath::Abs(a.Y - b.Y));
}

int32 UHFABoard::GetNeighbourCount(FNodeRef NodeRef) const
{
	return 8;
}

bool UHFABoard::IsValidRef(FNodeRef NodeRef) const
{
	return IsValid(NodeRef);
}

UHFABoard::FNodeRef UHFABoard::GetNeighbour(const FNodeRef NodeRef, const int32 NeiIndex) const
{
	return NodeRef + BoardNeightbors[NeiIndex];
}