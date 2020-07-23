#include "Util/HFABlueprintFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"

void UHFABlueprintFunctionLibrary::DebugDraw(const UObject* worldContextObject, const TArray<FVector>& points, float time)
{
#if WITH_EDITOR
	if (worldContextObject == nullptr) return;
	for (int i = 0; i < points.Num() - 1; i++)
	{
		DrawDebugLine(worldContextObject->GetWorld(), points[i], points[i + 1], FColor::Emerald, false, time);
	}
#endif
}

UNiagaraComponent* UHFABlueprintFunctionLibrary::SpawnSystemAtLocation(const UObject* WorldContextObject, class UNiagaraSystem* SystemTemplate, FVector Location, FRotator Rotation, FVector Scale, bool bAutoDestroy, bool bAutoActivate, ENCPoolMethod PoolingMethod, bool bPreCullCheck)
{
	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, SystemTemplate, Location, Rotation, Scale, bAutoDestroy, bAutoActivate, PoolingMethod, bPreCullCheck);
}