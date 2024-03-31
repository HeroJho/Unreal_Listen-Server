// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "ABStatChangeItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABStatChangeItemData : public UABItemData
{
	GENERATED_BODY()
	
public:
	UABStatChangeItemData();

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	float MoveSpeed;
	UPROPERTY(EditAnywhere, Category = Stat)
	float BombPower;
	UPROPERTY(EditAnywhere, Category = Stat)
	float BombCnt;


};
