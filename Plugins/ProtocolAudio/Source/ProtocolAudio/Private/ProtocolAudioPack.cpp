#include "ProtocolAudioPack.h"
#include "Kismet/GameplayStatics.h"

UProtocolAudioPack::UProtocolAudioPack() : Super()
{
	_volume = 1;
	_pitch = 1;
}

void UProtocolAudioPack::PlayDefaultSound(const UObject* worldContextObject, FVector location)
{
	if (_defaultSound == nullptr) return;
	UGameplayStatics::PlaySoundAtLocation(worldContextObject->GetWorld(), _defaultSound, location, _volume, _pitch);
}

void UProtocolAudioPack::PlaySoundAtLocation(const UObject* worldContextObject, FName hashName, FVector location)
{
	if (worldContextObject == nullptr) return;
	auto** ppack = _soundMap.Find(hashName);
	if (ppack == nullptr)
	{
		PlayDefaultSound(worldContextObject, location);
		return;
	}
	auto* pack = *ppack;
	if (pack->IsLooping())
	{
		UE_LOG(LogAudio, Warning, TEXT("[UProtocolAudioPack]: Pack %s tried to spawn infinitely looping sound asset %s. Spawning suppressed."), *GetNameSafe(this), *GetNameSafe(pack));
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(worldContextObject->GetWorld(), pack, location, _volume, _pitch);
}

void UProtocolAudioPack::SpawnSoundAttached(const UObject* worldContextObject, FName hashName, FVector location)
{
	//TODO:
	//UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, AttachName, FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
}