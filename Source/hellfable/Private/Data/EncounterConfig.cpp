#include "Data/EncounterConfig.h"
#include "Kismet/GameplayStatics.h"

AEncounterConfig::AEncounterConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

FEncounterData AEncounterConfig::GetRandomEncounter()
{
	return Encounters.Num() > 0 ? Encounters[FMath::RandHelper(Encounters.Num())] : FEncounterData();
}