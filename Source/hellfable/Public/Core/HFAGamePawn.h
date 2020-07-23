#pragma once
//Engine
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AI/Navigation/NavQueryFilter.h"
#include "UI/HFACharacterWorldUI.h"
#include "HFAGamePawn.generated.h"
//Game

//Weak Engine
class USkeletalMeshComponent;
class UCapsuleComponent;
class UNavigationSystemV1;
class UNavigationQueryFilter;
class GameplayStatics;

//Weak Game
class UHFAGamePawnWidgetComponent;
class UHFAStateMachineComponent;
class AHFAGameMode;
class UHFABoard;
class UBoardController;
class UProtocolAudioPack;

UENUM(BlueprintType)
enum class EGamePawnTeam : uint8
{
	UNINITIALIZED = 0,
	PLAYER,
	ENEMY
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	MELEE = 0,
	RANGED
};

UENUM(BlueprintType)
enum class EAttackArea : uint8
{
	SINGLE = 0,
	AREA
};

UENUM(BlueprintType)
enum class EOrderType : uint8
{
	MOVE_LOCATION = 0,
	MOVE_TARGET
};

USTRUCT()
struct FNavCurrentOrder
{
	GENERATED_USTRUCT_BODY()
public:
	EOrderType Type;
	AActor* Target;
	FIntPoint Node;
	bool IsActive;
	float OrderTime;

	void Reset();
};

USTRUCT()
struct FNavCurrentTarget
{
	GENERATED_USTRUCT_BODY()
public:
	EOrderType Type;
	AActor* Target;
	FVector Location;
	bool IsActive;

	void Reset();
};

UCLASS()
class HELLFABLE_API AHFAGamePawn : public APawn
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageCaused);
public:
	/** Default Constructor */
	AHFAGamePawn(const FObjectInitializer& ObjectInitializer);
	void PostInitProperties() override;
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region COMPONENTS
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UHFAStateMachineComponent* HFAStateMachineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UHFAGamePawnWidgetComponent* WidgetComponentWorld;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UActorComponent* ShieldParticle;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BATTLEUI")
	UTexture2D* BattlePortrait;

	/** Default Initializer */
	void BeginPlay() override;

	/** Default Tick */
	void Tick(float DeltaTime) override;

	/** Default Setup Input */
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure)
	FVector GetFeetPosition() const;

	UFUNCTION(BlueprintCallable)
	void SetFeetLocation(const FVector& targetPosition, bool sweep = false);

	UFUNCTION(BlueprintPure)
	bool IsAlive();

	UFUNCTION(BlueprintPure)
	bool IsDead();

	UFUNCTION(BlueprintPure)
	bool IsInBattle();

	UFUNCTION(BlueprintPure)
	EGamePawnTeam GetTeam();

	UFUNCTION(BlueprintPure)
	EAttackType GetAttackType();

	UFUNCTION(BlueprintPure)
	EAttackArea GetAttackArea();

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(AHFAGamePawn* target);

	/** Cancels current moviment order and teleport character to target position*/
	UFUNCTION(BlueprintCallable)
	void Teleport(const FIntPoint& target);

	UFUNCTION(BlueprintCallable)
	void ReceiveShield(float shield);

	UFUNCTION(BlueprintCallable)
	void ChargeEnergy(float damage);

	UFUNCTION(BlueprintCallable)
	void CauseDamage(float damage, FName damageType);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCauseDamage();

	UPROPERTY(BlueprintAssignable)
	FOnDamageCaused OnDamageCaused;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TriggerSpecial();

	UFUNCTION(BlueprintPure)
	bool IsInAttackDistance(AHFAGamePawn* target);

	UFUNCTION(BlueprintPure)
	bool IsMoving();
private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _navAgentSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _navAgentMinTimeBetweenOrders;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool _navDebugDrawLines;

	//GameMode
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHFAGameMode* _gameMode;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHFABoard* _board;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoardController* _boardController;

	//Navigation
	FNavCurrentOrder _navMoveOrder;
	FNavCurrentTarget _navCurrentTarget;
	TQueue<FVector> _navPoints;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _damageBase;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _maxHealth;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _energy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _maxEnergy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _shield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _maxShield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _increaseEnergyPerDamageMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EGamePawnTeam _team;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EAttackType _attackType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EAttackArea _attackArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool _hasSpecialAttack;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool _onBattleGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UProtocolAudioPack> _damagePack;

	void TickOrder();
	void TickMovement(float deltatime);
};
