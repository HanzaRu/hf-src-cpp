#pragma once
//Engine
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
//Game
#include "HFAPlayerHUD.generated.h"

/**
 * Player HUD class
 */
UCLASS()
class HELLFABLE_API AHFAPlayerHUD : public AHUD
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, meta = (WorldContext = "worldContextObject", UnsafeDuringActorConstruction = "true"))
	static AHFAPlayerHUD* GetHFAPlayerHUD(const UObject* worldContextObject, int playerIndex);
};
