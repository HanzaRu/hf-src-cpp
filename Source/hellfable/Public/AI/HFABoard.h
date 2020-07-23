#pragma once
//Engine
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
//Game
#include "HFABoard.generated.h"

/**
 * Board Graph
 */
UCLASS(BlueprintType, Blueprintable)
class HELLFABLE_API UHFABoard : public UObject
{
	GENERATED_BODY()
public:
	/** default initializer */
	UHFABoard(const FObjectInitializer& ObjectInitializer);

	/** default post init properties */
	void PostInitProperties() override;

	/** query and returns the nearest adjacent position on target to direction (target - from). */
	void AddPawn(AActor* actor);

	/** query and returns the nearest adjacent position on target to direction (target - from). */
	void RemovePawn(AActor* actor);

	/** query and returns the nearest adjacent position on target to direction (target - from). */
	UFUNCTION(BlueprintPure)
	bool QueryAdjacentPosition(AActor* actor, FIntPoint target, FIntPoint& out);

	/** query and returns the nearest adjacent position on target to direction (target - from). */
	UFUNCTION(BlueprintPure)
	bool QueryAdjacentInvertedPosition(AActor* actor, FIntPoint target, FIntPoint& out);

	/** returns a valid position if actor is inside board. */
	UFUNCTION(BlueprintPure)
	bool FindPosition(AActor* actor, FIntPoint& out);

	/** returns a path between start and end if possible. */
	UFUNCTION(BlueprintPure)
	bool FindPath(AActor* actor, FIntPoint end, TArray<FIntPoint>& navpoints);

	UFUNCTION(BlueprintPure)
	bool FindPathInWorld(AActor* actor, FIntPoint end, TArray<FVector>& navpoints);

	/** returns if position is open. */
	bool IsOpen(FIntPoint position) const;

	/** returns if position is open from pawn. */
	bool IsPawnOpen(AActor* actor,FIntPoint position) const;

	/** returns if position is closed. */
	bool IsClosed(FIntPoint position) const;

	/** returns if position is closed by a pawn. */
	bool IsPawnClosed(AActor* actor, FIntPoint position) const;

	/** Check if board position is valid. */
	bool IsValid(FIntPoint position) const;

	/** Check if board position is not valid. */
	bool IsNotValid(FIntPoint position) const;

	/** Board to World position. */
	UFUNCTION(BlueprintPure)
	FVector GetWorldPosition(FIntPoint point);

	/** calculates manhattan distance. */
	static int ManhattanDistance(FIntPoint a, FIntPoint b);

	/** calculates diagonal distance. */
	static int DiagonalDistance(FIntPoint a, FIntPoint b);

#pragma region FGraphAStar
	//defines FNodeRef type
	typedef FIntPoint FNodeRef;

	int32 GetNeighbourCount(FNodeRef NodeRef) const;
	bool IsValidRef(FNodeRef NodeRef) const;
	FNodeRef GetNeighbour(const FNodeRef NodeRef, const int32 NeiIndex) const;
#pragma endregion
private:
	UPROPERTY(EditDefaultsOnly)
	FVector _offset;

	UPROPERTY(EditDefaultsOnly)
	float _nodeSize;

	UPROPERTY(EditDefaultsOnly)
	int _size;

	FVector _uppertop;
	FVector _origin;
	TSet<FIntPoint> _closed;
	TMap<FIntPoint,AActor*> _pawnsclosed;
	TSet<AActor*> _pawns;

	/** World to Board position. */
	FIntPoint GetBoardPosition(FVector position);

	/** Update closed list of pawns */
	void UpdateClosedPawnList();
};
