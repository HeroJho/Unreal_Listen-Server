// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "ABCharacterNonePlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_CPP_API AABCharacterNonePlayer : public AABCharacterBase
{
	GENERATED_BODY()

public:
	AABCharacterNonePlayer();

protected:
	void SetDead() override;

};
