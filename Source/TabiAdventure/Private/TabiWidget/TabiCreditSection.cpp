// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.


#include "TabiWidget/TabiCreditSection.h"
#include "TabiWidget/TabiCreditEntry.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UTabiCreditSection::SetSection(const FTabiCreditSectionData& Section)
{
	if (!SectionHeader || !EntryList) return;

	SectionHeader->SetText(Section.Header);
	SectionHeader->SetVisibility(Section.Header.IsEmpty()
		                             ? ESlateVisibility::Collapsed
		                             : ESlateVisibility::SelfHitTestInvisible
	);

	EntryList->ClearChildren();

	if (!ensure(CreditEntryClass)) return;

	for (const FTabiCreditEntryData& Entry : Section.Entries)
	{
		UTabiCreditEntry* NewEntry = CreateWidget<UTabiCreditEntry>(this, CreditEntryClass);
		if (!NewEntry) return;

		NewEntry->SetEntry(Entry);

		if (UVerticalBoxSlot* EntrySlot = EntryList->AddChildToVerticalBox(NewEntry))
		{
			EntrySlot->SetPadding(FMargin(0.f, 0.f, 0.f, EntrySpacing));
		}
	}
}
