#pragma once
//Engine
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//Game
#include "ProtocolUIWidget.generated.h"

/**
 * Protocol UI base UserWidget class.
 */
UCLASS()
class PROTOCOLUI_API UProtocolUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UProtocolUIWidget(const FObjectInitializer& ObjectInitializer);

	void SetWidgetVisibility(bool visible);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShow();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHide();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> VisibleStates;
protected:
	bool _isVisible;
	bool _isInitialized;
};
