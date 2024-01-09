// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonePlayer.h"

AABCharacterNonePlayer::AABCharacterNonePlayer()
{
}

void AABCharacterNonePlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
