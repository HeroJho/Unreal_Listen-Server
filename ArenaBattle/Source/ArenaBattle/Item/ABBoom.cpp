// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABBoom.h"
#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AABBoom::AABBoom()
	:BoomTime(0.f) 
	,AttackRadius(0.f)
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

	bReplicates = true;
}

// Called when the game starts or when spawned
void AABBoom::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		FTimerHandle Handle;
		Collision->SetSimulatePhysics(true);
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AABBoom::NetMulticastRPC_Boom, BoomTime, false);
	}

}

// Called every frame
void AABBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AABBoom::NetMulticastRPC_Boom_Implementation()
{
	Effect->Activate(true);
	Effect->OnSystemFinished.AddDynamic(this, &AABBoom::OnEffectFinished);

	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	// 서버에서만 체크
	if (HasAuthority())
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
		TArray<FOverlapResult> HitActors;
		bool HitDetected = GetWorld()->OverlapMultiByChannel(HitActors, GetActorLocation(), FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

		if (HitDetected)
		{
			for (int32 i = 0; i < HitActors.Num(); ++i)
			{
				AActor* HitActor = HitActors[i].GetActor();

				FDamageEvent DamageEvent;
				HitActor->TakeDamage(10, DamageEvent, nullptr, this);
			}
		}

#if ENABLE_DRAW_DEBUG
		DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRadius, 12, FColor::Cyan, false, 5.0f);
#endif

	}
	
}

void AABBoom::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

