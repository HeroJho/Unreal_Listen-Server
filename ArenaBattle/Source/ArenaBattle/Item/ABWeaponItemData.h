// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "ABWeaponItemData.generated.h"


UENUM()
enum class EUseableItemID : uint32
{
	Jump = 1,
	Map,
	Hide,
	END
};


UCLASS()
class ARENABATTLE_API UABWeaponItemData : public UABItemData
{
	GENERATED_BODY()

public:
	UABWeaponItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	EUseableItemID ItemID;
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UStaticMesh> ItemMesh;
	UPROPERTY(EditAnywhere, Category = Weapon)
	FName MeshBoneName;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float Cooldown;

};
