#include "UI/HFAPlayerHUD.h"
//Engine
#include "Kismet/GameplayStatics.h"

AHFAPlayerHUD* AHFAPlayerHUD::GetHFAPlayerHUD(const UObject* worldContextObject, int playerIndex)
{
	return UGameplayStatics::GetPlayerController(worldContextObject, playerIndex)->GetHUD<AHFAPlayerHUD>();
}

