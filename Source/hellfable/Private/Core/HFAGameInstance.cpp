#include "Core/HFAGameInstance.h"
//Game

UHFAGameInstance* UHFAGameInstance::GetHFAGameInstance(const UObject* worldContextObject)
{
	return Cast<UHFAGameInstance>(worldContextObject->GetWorld()->GetGameInstance());
}