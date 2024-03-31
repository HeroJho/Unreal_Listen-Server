// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABSkillManager.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABSkillManager : public UObject
{
	GENERATED_BODY()

public:
	UABSkillManager();

public:
	void CreateBomb(AActor* OwnerActor, class UABCharacterStatComponent* Stat, FTransform SpawnTransform, int Power);

private:
	TSubclassOf<class AABBoom> BombClass;

};
