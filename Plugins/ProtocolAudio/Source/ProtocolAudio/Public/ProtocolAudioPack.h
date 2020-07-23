#pragma once
//Engine
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//Game
#include "ProtocolAudioPack.generated.h"

/**
 * Package of One-shot sounds. Play it requires a valid registered FName.
 */
UCLASS(BlueprintType, Blueprintable)
class PROTOCOLAUDIO_API UProtocolAudioPack : public UObject
{
	GENERATED_BODY()
public:
	UProtocolAudioPack();

	UFUNCTION(BlueprintCallable)
	void PlayDefaultSound(const UObject* worldContextObject, FVector location);

	UFUNCTION(BlueprintCallable)
	void PlaySoundAtLocation(const UObject* worldContextObject, FName hashName, FVector location);

	UFUNCTION(BlueprintCallable)
	void SpawnSoundAttached(const UObject* worldContextObject, FName hashName, FVector location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProtocolAudioPack")
	TMap<FName, USoundBase*> _soundMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProtocolAudioPack")
	USoundBase* _defaultSound;

	// Volume Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProtocolAudioPack")
	float _volume;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProtocolAudioPack")
	float _pitch;
};
