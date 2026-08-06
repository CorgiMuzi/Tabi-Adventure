// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiButton.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTabiButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	ButtonSizeBox->SetWidthOverride(ButtonSize.X);
	ButtonSizeBox->SetHeightOverride(ButtonSize.Y);
	ButtonText->SetText(InButtonText);
}

void UTabiButton::NativeConstruct()
{
	Super::NativeConstruct();

	TargetButton->OnReleased.AddDynamic(this, &UTabiButton::OnButtonReleased);
}


void UTabiButton::OnButtonReleased()
{
	OnTabiButtonReleased.Broadcast();
}
