#pragma once
//Engine
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//Game
#include "Core/HFAGamePawn.h"
#include "HFAGameInstance.generated.h"


/** PlayerSaveData */
USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> TeamMembers;
};

/**
 * Game Instance contains Application Level Information
 */
UCLASS()
class HELLFABLE_API UHFAGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintPure, meta = (WorldContext = "worldContextObject"))
	static UHFAGameInstance* GetHFAGameInstance(const UObject* worldContextObject);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPlayerSaveData PlayerSaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pool)
	TArray<TSubclassOf<AHFAGamePawn>>  CharacterPool;
};
