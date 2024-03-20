// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABBoom.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AABBoom::AABBoom()
	:BoomTime(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Effect->bAutoActivate = false;

	Collision->SetCollisionProfileName(TEXT("PhysicsActor"));

}

// Called when the game starts or when spawned
void AABBoom::BeginPlay()
{
	Super::BeginPlay();
	Collision->SetSimulatePhysics(true);

	if (HasAuthority())
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AABBoom::Boom, BoomTime, false);
	}

}

// Called every frame
void AABBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABBoom::Boom()
{
	Effect->Activate(true);
	Effect->OnSystemFinished.AddDynamic(this, &AABBoom::OnEffectFinished);

	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

}

void AABBoom::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

