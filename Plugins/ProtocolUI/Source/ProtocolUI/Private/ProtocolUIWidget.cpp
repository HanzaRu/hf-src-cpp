#include "ProtocolUIWidget.h"

UProtocolUIWidget::UProtocolUIWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	_isVisible = false;
	_isInitialized = false;
}

void UProtocolUIWidget::SetWidgetVisibility(bool visible)
{
	if (_isVisible == visible && _isInitialized) return;
	_isVisible = visible;
	_isInitialized = true;
	SetVisibility(visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (visible)
	{
		OnShow();
	}
	else
	{
		OnHide();
	}
}

