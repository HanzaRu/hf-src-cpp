#include "Core/HFAStartMarker.h"
//Engine
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"

AHFAStartMarker::AHFAStartMarker(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->InitCapsuleSize(33, 66);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->SetGenerateOverlapEvents(false);
	RootComponent = CapsuleComponent;
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	BillboardComponent->MarkAsEditorOnlySubobject();
	BillboardComponent->SetupAttachment(CapsuleComponent);
}
