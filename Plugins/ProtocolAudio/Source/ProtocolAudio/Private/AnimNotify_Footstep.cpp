#include "AnimNotify_Footstep.h"
//Game
#include "ProtocolAudioPack.h"

UAnimNotify_Footstep::UAnimNotify_Footstep() : Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(196, 142, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

FString UAnimNotify_Footstep::GetNotifyName_Implementation() const
{
	return FString("Footstep");
}

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (Pack.GetDefaultObject() == nullptr) return;
	if (MeshComp->ComponentTags.Num() > 0)
	{
		for (auto tag : MeshComp->ComponentTags)
		{
			Pack.GetDefaultObject()->PlaySoundAtLocation(MeshComp, tag, MeshComp->GetComponentLocation());
		}
	}
	else
	{
		Pack.GetDefaultObject()->PlayDefaultSound(MeshComp, MeshComp->GetComponentLocation());
	}
}

