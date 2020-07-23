#pragma once
//Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntPoint.h"
//Game
#include "EncounterConfig.generated.h"

class AHFAGamePawn;

USTRUCT(BlueprintType)
struct FEncounterEnemyConfig
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHFAGamePawn> EnemyClass;
};

USTRUCT(BlueprintType)
struct FEncounterData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEncounterEnemyConfig> Enemies;
};

/**
 * DataAsset for Configure Map Encounters;
 */
UCLASS()
class HELLFABLE_API AEncounterConfig : public AActor
{
	GENERATED_BODY()
public:
	/** Default Constructor */
	AEncounterConfig(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEncounterData> Encounters;

	UFUNCTION(BlueprintPure)
	FEncounterData GetRandomEncounter();
};
