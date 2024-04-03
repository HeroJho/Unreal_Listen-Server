// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ABItemDropData.h"
#include "ABItemData.h"
#include "Game/ABGameMode.h"
#include "Manager/ABItemManager.h"


// Sets default values
AABBox::AABBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	
	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AABBox::BeginPlay()
{
	Super::BeginPlay();

	if (DropInfo)
	{
		for (auto Num : DropInfo->ItemPercentageInfos)
		{
			DropTotalPercentage += Num;
		}
	}

}

// Called every frame
void AABBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AABBox::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	BreakBox();

	return ActualDamage;
}

void AABBox::DropItem()
{
	// 서버 로직
	if (!HasAuthority())
		return;

	if (DropInfo == nullptr)
		return;

	// 확률 뽑기
	const float DropPercen = FMath::RandRange(0.f, DropTotalPercentage);
	float DropPercenAcc = 0.f;
	TObjectPtr<UABItemData> DropItemInfo = nullptr;
	for (int32 i = 0; i < DropInfo->ItemPercentageInfos.Num(); ++i)
	{
		float CurNum = DropInfo->ItemPercentageInfos[i];

		DropPercenAcc += CurNum;
		if (DropPercen <= DropPercenAcc)
		{
			DropItemInfo = DropInfo->ItemInfos[i];
			break;
		} 

	}

	if (DropItemInfo == nullptr)
		return;

	// 아이템 생성
	AABGameMode* ABGameMode = GetWorld()->GetAuthGameMode<AABGameMode>();
	if (ABGameMode)
	{
		FTransform SpawnTransform = GetActorTransform();

		if (ABGameMode->GetItemManager())
		{
			ABGameMode->GetItemManager()->CreateItem(DropItemInfo, SpawnTransform);
		}

	}

}

void AABBox::BreakBox()
{
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	DropItem();

	FTransform EffectTransform;
	EffectTransform.SetScale3D({ 2.f, 2.f, 2.f });
	EffectTransform.SetLocation(GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BreakParticle, EffectTransform);

	Destroy();
}

