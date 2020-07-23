#pragma once
//Engine
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
//Game
#include "AnimNotify_Footstep.generated.h"

class UProtocolAudioPack;

/**
 * Anim Class to play footsteps. Uses SkeletonMeshComponent tag to dispatch correct sound.
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Play Footstep"))
class PROTOCOLAUDIO_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()
public:
	/** default constructor */
	UAnimNotify_Footstep();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	// Pack to Play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	TSubclassOf<UProtocolAudioPack> Pack;
};
