// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABBoom.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AABBoom::AABBoom()
	:BoomTime(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	Collision->SetCollisionProfileName(TEXT("PhysicsActor"));

}

// Called when the game starts or when spawned
void AABBoom::BeginPlay()
{
	Super::BeginPlay();
	Collision->SetSimulatePhysics(true);

	BoomTime = 5.f;

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AABBoom::Boom, BoomTime, false);
}

// Called every frame
void AABBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABBoom::Boom()
{
	Destroy();
}

