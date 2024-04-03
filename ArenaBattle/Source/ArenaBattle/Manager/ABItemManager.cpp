// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ABItemManager.h"
#include "Item/ABItem.h"

UABItemManager::UABItemManager()
{
	static ConstructorHelpers::FClassFinder<AABItem> ItemClassRef(TEXT("/Script/Engine.Blueprint'/Game/ArenaBattle/Blueprint/Item/BP_Item.BP_Item_C'"));
	if (ItemClassRef.Class)
	{
		ItemClass = ItemClassRef.Class;
	}
}

void UABItemManager::CreateItem(TObjectPtr<UABItemData> InItemData, const FTransform& InSpawnTransform)
{

	AABItem* Item = GetWorld()->SpawnActor<AABItem>(ItemClass, InSpawnTransform);
	if (Item)
	{
		Item->SetProperty(InItemData);
	}

}
