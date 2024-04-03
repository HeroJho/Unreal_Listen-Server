// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemDropData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABItemDropData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UABItemData>> ItemInfos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ItemPercentageInfos;

};
