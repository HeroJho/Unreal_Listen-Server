// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABItemManager.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABItemManager : public UObject
{
	GENERATED_BODY()
	
public:
	UABItemManager();

public:
	void CreateItem(TObjectPtr<class UABItemData> InItemData, const FTransform& InSpawnTransform);

protected:
	UPROPERTY()
	TSubclassOf<class AABItem> ItemClass;

};
