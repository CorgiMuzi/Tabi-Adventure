// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiCreditEntry.h"

#include "Components/TextBlock.h"

void UTabiCreditEntry::SetEntry(const FTabiCreditEntryData& Entry)
{
	if (!EntryLabel || !EntryValue) return;

	EntryLabel->SetText(Entry.Label);
	EntryLabel->SetVisibility(Entry.Label.IsEmpty()
		                          ? ESlateVisibility::Collapsed
		                          : ESlateVisibility::SelfHitTestInvisible
	);

	EntryValue->SetText(Entry.Value);
}
