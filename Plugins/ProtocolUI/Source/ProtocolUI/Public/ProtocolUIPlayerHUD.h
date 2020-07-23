#pragma once
//Engine
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
//ProtocolUI
#include "ProtocolUI.h"
#include "ProtocolUIPlayerHUD.generated.h"
//Weak
class UProtocolUIWidget;

UCLASS()
class UProtocolUIState : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	bool IsActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	int Layer;
};

/**
 * Generic state-based UI generator and caller
 */
UCLASS()
class PROTOCOLUI_API AProtocolUIPlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	AProtocolUIPlayerHUD();

	UFUNCTION(BlueprintPure, Category = "ProtocolUI", meta = (WorldContext = "WorldContextObject"))
	static AProtocolUIPlayerHUD* GetProtocolUIPlayerHUD(const UObject * worldContextObject, int index = 0);
	
	/** Default Initializer */
	virtual void BeginPlay() override;

	/** Default tick */
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ProtocolUI")
	void SetState(FName state, bool enabled);

	UFUNCTION(BlueprintPure, Category = "ProtocolUI")
	UProtocolUIState* GetState(FName state);

	UFUNCTION(BlueprintCallable, Category = "ProtocolUI")
	void SetupWidget(TArray<TSubclassOf<UProtocolUIWidget>> widgets, int zOrder);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName, int> StateConfigs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> ActiveStates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> EditorActiveStates;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName, UProtocolUIState*> States;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UProtocolUIWidget*> StateWidgets;

	/** states was rebuilt. */
	bool isRebuilt;

	/** internal process all state widgets. */
	void ProcessStates();

	/** internal process a state widget. */
	void ProcessState(UProtocolUIWidget* widget);
};
