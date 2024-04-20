// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ABWeaponItemData.h"
#include "ABCharacterCooldownItemComponent.generated.h"


DECLARE_DELEGATE(FOnUseItemDelegate);
USTRUCT(BlueprintType)
struct FUseableItemDelegateWrapper
{
	GENERATED_BODY()
	FUseableItemDelegateWrapper() {}
	FUseableItemDelegateWrapper(const FOnUseItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnUseItemDelegate ItemDelegate;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterCooldownItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterCooldownItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetItemData(class UABWeaponItemData* InItemData);
	void SetUseableItemDeletage(EUseableItemID ID, FOnUseItemDelegate Deletage);

	void UseItem();

protected:
	EUseableItemID ItemID;
	float Cooldown;
	float CooldownLastTime;

	UPROPERTY()
	TArray<FUseableItemDelegateWrapper> UseableItemActions;

};
