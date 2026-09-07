// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiData/TabiCreditData.h"
#include "TabiCreditSection.generated.h"

class UTextBlock;
class UVerticalBox;
class UTabiCreditEntry;

UCLASS()
class TABIADVENTURE_API UTabiCreditSection : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSection(const FTabiCreditSectionData& Section);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTabiCreditEntry> CreditEntryClass;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> SectionHeader;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> EntryList;
	
	UPROPERTY(EditDefaultsOnly)
	float EntrySpacing = 2.f;
};
