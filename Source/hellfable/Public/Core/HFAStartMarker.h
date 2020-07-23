#pragma once
//Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//Game
#include "HFAStartMarker.generated.h"
//Weak
class UBillboardComponent;
class UCapsuleComponent;

UENUM(BlueprintType)
enum class EStartMakerType : uint8
{
	NONE = 0,
	CHARACTER,
	ENEMY,
};

UCLASS()
class HELLFABLE_API AHFAStartMarker : public AActor
{
	GENERATED_BODY()
public:
	/** Default Constructor */
	AHFAStartMarker(const FObjectInitializer& ObjectInitializer);
	
	/** Components */
	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category=Component, VisibleAnywhere, BlueprintReadOnly)
	UBillboardComponent* BillboardComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EStartMakerType StartMakerType = EStartMakerType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int StartMakerNumber = 0;
};
