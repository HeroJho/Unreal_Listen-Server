// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItem.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/ABCharacterItemInterface.h"
#include "ABItemData.h"

// Sets default values
AABItem::AABItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void AABItem::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
		Mesh->SetCollisionProfileName("ABItem");
		Mesh->SetSimulatePhysics(true);
	}

}

void AABItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AABItem::OnOverlapBegin);
}

void AABItem::PostNetInit()
{
	Super::PostNetInit();

	OnRep_ItemData();

}

void AABItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABItem, ItemData);
}

void AABItem::SetProperty(TObjectPtr<UABItemData> InItemData)
{
	ItemData = InItemData;
	OnRep_ItemData();
}

void AABItem::DestroyItem()
{
	Destroy();
}

void AABItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{	
	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(ItemData);
	}


	FTransform EffectTransform;
	// EffectTransform.SetScale3D(Vector);
	EffectTransform.SetLocation(GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, EffectTransform);

	FTimerHandle EffectHandle;
	GetWorldTimerManager().SetTimer(EffectHandle, this, &AABItem::DestroyItem, 2.f, false);

	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	
}

void AABItem::OnRep_ItemData()
{
	Mesh->SetStaticMesh(ItemData->Mesh);
}



