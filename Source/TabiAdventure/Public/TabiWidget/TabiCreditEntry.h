// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabiData/TabiCreditData.h"
#include "TabiCreditEntry.generated.h"

class UTextBlock;

UCLASS()
class TABIADVENTURE_API UTabiCreditEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetEntry(const FTabiCreditEntryData& Entry);
	
protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> EntryLabel; 
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> EntryValue;
};
