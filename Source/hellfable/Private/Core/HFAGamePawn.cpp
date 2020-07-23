#include "Core/HFAGamePawn.h"
//Engine
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystemTypes.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
//Game
#include "AI/HFAStateMachineComponent.h"
#include "AI/HFABoard.h"
#include "Core/HFAGameMode.h"
#include "Core/Controllers/BoardController.h"
#include "UI/HFAGamePawnWidgetComponent.h"
#include "Util/HFABlueprintFunctionLibrary.h"
//Plugin
#include "ProtocolAudioPack.h"

void FNavCurrentOrder::Reset()
{
	Type = EOrderType::MOVE_LOCATION;
	Target = nullptr;
	Node = FIntPoint::ZeroValue;
	IsActive = false;
	OrderTime = 0;
}

void FNavCurrentTarget::Reset()
{
	Type = EOrderType::MOVE_LOCATION;
	Target = nullptr;
	Location = FVector::ZeroVector;
	IsActive = false;
}

AHFAGamePawn::AHFAGamePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	_maxHealth = 100;
	_maxEnergy = 100;
	_onBattleGround = false;
	_hasSpecialAttack = false;

	_gameMode = nullptr;
	_navCurrentTarget.Reset();
	_navMoveOrder.Reset();
	_navPoints.Empty();
	_navAgentSpeed = 300.0f;
	_navAgentMinTimeBetweenOrders = 1 / 20.0f; //20 orders per second;

	_navDebugDrawLines = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	SkeletalMeshComponent->SetRelativeLocation(FVector(0, 0, -CapsuleComponent->GetScaledCapsuleHalfHeight()));

	WidgetComponentWorld = CreateDefaultSubobject<UHFAGamePawnWidgetComponent>("GamePawnWidgetComponent");
	WidgetComponentWorld->SetupAttachment(CapsuleComponent);
	WidgetComponentWorld->SetRelativeLocation(GetActorLocation() + FVector(0, 0, CapsuleComponent->GetScaledCapsuleHalfHeight() + 50));
	WidgetComponentWorld->SetRelativeRotation(FRotator(0, 0, 0));

	HFAStateMachineComponent = CreateDefaultSubobject<UHFAStateMachineComponent>("HFAStateMachineComponent");
}

void AHFAGamePawn::PostInitProperties()
{
	Super::PostInitProperties();
	SkeletalMeshComponent->SetRelativeLocation(FVector(0, 0, -CapsuleComponent->GetScaledCapsuleHalfHeight()));
}

#if WITH_EDITOR
void AHFAGamePawn::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SkeletalMeshComponent->SetRelativeLocation(FVector(0, 0, -CapsuleComponent->GetScaledCapsuleHalfHeight()));
}
#endif

void AHFAGamePawn::BeginPlay()
{
	Super::BeginPlay();
	_gameMode = AHFAGameMode::GetHFAGameMode(this);
	_boardController = _gameMode->BoardController;
	_board = _boardController->GetBoard();
	_board->AddPawn(this);
	_health = _maxHealth;
	_energy = 0;
}

void AHFAGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsInBattle()) return;
	if (_gameMode->CurrentBattleState != EBattleState::BATTLE) return;
	TickOrder();
	TickMovement(DeltaTime);
	WidgetComponentWorld->SetWorldRotation(FRotator(45, -180, 0));
}

void AHFAGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector AHFAGamePawn::GetFeetPosition() const
{
	return GetActorLocation() - FVector(0, 0, CapsuleComponent->GetScaledCapsuleHalfHeight());
}

bool AHFAGamePawn::IsAlive()
{
	return _health > 0;
}

bool AHFAGamePawn::IsDead()
{
	return !IsAlive();
}

bool AHFAGamePawn::IsInBattle()
{
	return _onBattleGround;
}

EGamePawnTeam AHFAGamePawn::GetTeam()
{
	return _team;
}

EAttackType AHFAGamePawn::GetAttackType()
{
	return _attackType;
}

EAttackArea AHFAGamePawn::GetAttackArea()
{
	return _attackArea;
}

void AHFAGamePawn::ReceiveShield(float shield)
{
	_shield = shield;
	_maxShield = shield;
}

void AHFAGamePawn::ChargeEnergy(float damage)
{
	_energy += damage * _increaseEnergyPerDamageMultiplier;
	if (_energy <= _maxEnergy) return;
	_energy = _maxEnergy;
}

void AHFAGamePawn::CauseDamage(float damage, FName damageType)
{
	if (_health > 0)
	{
		//before become dead
		OnCauseDamage();
		OnDamageCaused.Broadcast();
		//play audio
		if (_damagePack != nullptr)
		{
			_damagePack.GetDefaultObject()->PlaySoundAtLocation(this, damageType, GetActorLocation());
		}

		if (_shield > 0)
		{
			_shield -= damage;
			if (_shield < 0)
			{
				_health -= FMath::Abs(_shield);
				_shield = 0;
			}
		}
		else
		{
			_health -= damage;
		}

		if (_hasSpecialAttack)
		{
			ChargeEnergy(damage);
		}

		if (_health <= 0)
		{
			_health = 0;
			_board->RemovePawn(this);
			HFAStateMachineComponent->ChangeState(FName("dead"));
			HFAStateMachineComponent->SetEnabled(false);
			WidgetComponentWorld->SetVisibility(false, true);
			_gameMode->VerifyConditionToEndBattle();
		}
	}
}

bool AHFAGamePawn::IsInAttackDistance(AHFAGamePawn* target)
{
	return _boardController->IsInAttackDistance(this, target);
}

bool AHFAGamePawn::IsMoving()
{
	return _navCurrentTarget.IsActive;
}

bool AHFAGamePawn::TriggerSpecial_Implementation()
{
	if (_energy >= _maxEnergy)
	{
		_energy = 0;
		HFAStateMachineComponent->ChangeState("special");
		return true;
	}
	else
	{
		return false;
	}
}

void AHFAGamePawn::MoveToTarget(AHFAGamePawn* target)
{
	if (target == nullptr) return;
	//build order
	if (!_navMoveOrder.IsActive) _navMoveOrder.OrderTime = GetWorld()->TimeSeconds;
	_navMoveOrder.Type = EOrderType::MOVE_TARGET;
	_navMoveOrder.Target = target;
	_navMoveOrder.IsActive = true;
}

void AHFAGamePawn::Teleport(const FIntPoint& target)
{
	if (!_board->IsValid(target)) return;
	_navMoveOrder.Reset();
	_navCurrentTarget.Reset();
	_navPoints.Empty();
	SetFeetLocation(_board->GetWorldPosition(target));
}

void AHFAGamePawn::SetFeetLocation(const FVector& targetPosition, bool sweep)
{
	if (SetActorLocation(targetPosition + FVector(0, 0, CapsuleComponent->GetScaledCapsuleHalfHeight()), sweep)) return;
	UE_LOG(LogTemp, Warning, TEXT("[AHFAGamePawn::SetFeetLocation] Sweep failed."));
}

void AHFAGamePawn::TickOrder()
{
	if (!_navMoveOrder.IsActive) return;
	if (GetWorld()->TimeSeconds < _navMoveOrder.OrderTime + _navAgentMinTimeBetweenOrders) return;
	_navMoveOrder.IsActive = false;
	FIntPoint f;
	if (!_board->FindPosition(this, f)) return;
	switch (_navMoveOrder.Type)
	{
		case EOrderType::MOVE_TARGET:
			FIntPoint t;
			if (!_board->FindPosition(_navMoveOrder.Target, t)) return;
			if (!_board->QueryAdjacentPosition(this, t, _navMoveOrder.Node)) return;
			break;
	}

	_navPoints.Empty();
	if (f == _navMoveOrder.Node)
	{
		_navCurrentTarget.Type = _navMoveOrder.Type;
		_navCurrentTarget.Target = _navMoveOrder.Target;
		_navCurrentTarget.Location = _board->GetWorldPosition(_navMoveOrder.Node);
		_navCurrentTarget.IsActive = true;
		return;
	}
	static TArray<FVector> path;
	path.Reset();
	if (!_board->FindPathInWorld(this, _navMoveOrder.Node, path)) return;

	//update navCurrentHeaders
	_navCurrentTarget.Type = _navMoveOrder.Type;
	_navCurrentTarget.Target = _navMoveOrder.Target;

	for (int i = 0; i < path.Num(); i++)
	{
		_navPoints.Enqueue(path[i]);
	}
#if WITH_EDITOR
	if (_navDebugDrawLines)
	{
		UHFABlueprintFunctionLibrary::DebugDraw(this, path, 2);
	}
#endif
}

void AHFAGamePawn::TickMovement(float deltatime)
{
	//Check for new targets
	if (!_navCurrentTarget.IsActive) _navCurrentTarget.IsActive = _navPoints.Dequeue(_navCurrentTarget.Location);
	//No active navCurrentTarget
	if (!_navCurrentTarget.IsActive) return;
	//walk through the distance squared
	auto maxDistanceSquared = _navAgentSpeed * deltatime * _navAgentSpeed * deltatime;
	for (auto distanceSquared = 0; distanceSquared < maxDistanceSquared;)
	{
		auto remainingDistanceSquared = maxDistanceSquared - distanceSquared;
		auto currentLocation = GetFeetPosition();
		auto offset = _navCurrentTarget.Location - currentLocation;
		auto offsetSquared = offset.SizeSquared();
		if (offsetSquared < remainingDistanceSquared)
		{
			SetFeetLocation(_navCurrentTarget.Location, true);
			SetActorRotation(UKismetMathLibrary::MakeRotFromX(offset));
			//dequeue a new location or stop
			_navCurrentTarget.IsActive = _navPoints.Dequeue(_navCurrentTarget.Location);
			if (!_navCurrentTarget.IsActive)
			{
				//set final rotation
				if (_navCurrentTarget.Type == EOrderType::MOVE_TARGET)
				{
					FVector planeXY = (_navCurrentTarget.Target->GetActorLocation() - GetActorLocation());
					planeXY.Z = 0;
					SetActorRotation(UKismetMathLibrary::MakeRotFromX(planeXY.GetSafeNormal()));
				}
				return;
			}
			distanceSquared += offsetSquared;
		}
		else
		{
			SetActorRotation(UKismetMathLibrary::MakeRotFromX(offset));
			SetFeetLocation(currentLocation + offset.GetSafeNormal() * FMath::Sqrt(remainingDistanceSquared), true);
			return;
		}
	}
}
