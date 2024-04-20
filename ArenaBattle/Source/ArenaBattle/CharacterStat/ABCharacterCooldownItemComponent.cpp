// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterCooldownItemComponent.h"

// Sets default values for this component's properties
UABCharacterCooldownItemComponent::UABCharacterCooldownItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CooldownLastTime = 0.f;
	ItemID = EUseableItemID::END;
	UseableItemActions.SetNum((uint32)EUseableItemID::END);
}


// Called when the game starts
void UABCharacterCooldownItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UABCharacterCooldownItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UABCharacterCooldownItemComponent::SetItemData(UABWeaponItemData* InItemData)
{
	CooldownLastTime = 0.f;
	Cooldown = InItemData->Cooldown;
	ItemID = InItemData->ItemID;
}

void UABCharacterCooldownItemComponent::SetUseableItemDeletage(EUseableItemID ID, FOnUseItemDelegate Deletage)
{
	UseableItemActions[(uint32)ID] = FUseableItemDelegateWrapper(Deletage);
}

void UABCharacterCooldownItemComponent::UseItem()
{
	if (ItemID == EUseableItemID::END)
		return;

	const double DisTime = GetWorld()->GetTimeSeconds() - CooldownLastTime;
	if (DisTime > Cooldown)
	{
		CooldownLastTime = GetWorld()->GetTimeSeconds();
		UseableItemActions[(uint8)ItemID].ItemDelegate.ExecuteIfBound();
	}

}

