// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItem.h"
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
	

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);



}

// Called when the game starts or when spawned
void AABItem::BeginPlay()
{
	Super::BeginPlay();

	Collision->SetCollisionProfileName("NoCollision");
	// Collision->SetSimulatePhysics(true);

	Mesh->SetStaticMesh(ItemData->Mesh);
	Mesh->SetCollisionProfileName("ABItem");
	Mesh->SetSimulatePhysics(true);

}

void AABItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AABItem::OnOverlapBegin);
}

void AABItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{	
	if (nullptr == ItemData)
	{
		Destroy();
		return;
	}

	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(ItemData);
	}

	Destroy();
}



