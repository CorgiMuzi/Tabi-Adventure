// Copyright (c) 2026 CorgiMuzi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TabiCreditData.generated.h"

USTRUCT(BlueprintType)
struct FTabiCreditEntryData
{
	GENERATED_BODY()

	/** Role or asset name*/
	UPROPERTY(EditAnywhere, Category="Credit")
	FText Label;

	/** Person, author or notice text*/
	UPROPERTY(EditAnywhere, Category="Credit", meta=(MultiLine=true))
	FText Value;
};

USTRUCT(BlueprintType)
struct FTabiCreditSectionData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Credit")
	FText Header;

	UPROPERTY(EditAnywhere, Category="Credit")
	TArray<FTabiCreditEntryData> Entries;
};

UCLASS()
class TABIADVENTURE_API UTabiCreditData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Credit")
	TArray<FTabiCreditSectionData> Sections;
	
	UPROPERTY(EditDefaultsOnly, Category="Credit", meta=(MultiLine=true))
	FText ClosingComment;
};
