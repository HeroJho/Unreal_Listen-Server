// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterCooldownItemComponent.h"
#include "ArenaBattle.h"
#include "Net/UnrealNetwork.h"
#include "Character/ABCharacterBase.h"

// Sets default values for this component's properties
UABCharacterCooldownItemComponent::UABCharacterCooldownItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bCanUse = true;
	ItemID = EUseableItemID::END;
	UseableItemActions.SetNum((uint32)EUseableItemID::END);

	SetIsReplicated(true);
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
	EndCooldown();
	Cooldown = InItemData->Cooldown;
	ItemID = InItemData->ItemID;

}

void UABCharacterCooldownItemComponent::SetUseableItemDeletage(EUseableItemID ID, FOnUseItemDelegate Deletage)
{
	UseableItemActions[(uint32)ID] = FUseableItemDelegateWrapper(Deletage);
}

bool UABCharacterCooldownItemComponent::CheckUseableItem()
{
	return (ItemID != EUseableItemID::END) && bCanUse;
}

void UABCharacterCooldownItemComponent::UseItem()
{
	if (CheckUseableItem())
	{
		bCanUse = false;

		// 서버에서만 실행
		if (GetOwner()->HasAuthority())
		{
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UABCharacterCooldownItemComponent::EndCooldown, Cooldown, false);
			UseableItemActions[(uint8)ItemID].ItemDelegate.ExecuteIfBound();
		}
		else
		{
			ServerRPCUseItem();
		}
		
	}

}

void UABCharacterCooldownItemComponent::EndCooldown()
{
	bCanUse = true;
}

void UABCharacterCooldownItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UABCharacterCooldownItemComponent, bCanUse);
}

void UABCharacterCooldownItemComponent::OnRep_CanUse()
{
	// UI 갱신
}

void UABCharacterCooldownItemComponent::ServerRPCUseItem_Implementation()
{
	UseItem();
}







