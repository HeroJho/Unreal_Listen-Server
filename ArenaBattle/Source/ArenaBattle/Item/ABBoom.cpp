// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABBoom.h"
#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AABBoom::AABBoom()
	:BoomTime(0.f)
	, AttackRadius(0.f)
	, BoomLineDis(0)
	, BoomLineCnt(0)
	, EnableDir{ false }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Collision->SetCollisionProfileName(TEXT("PhysicsActor"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AABBoom::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	Collision->SetSimulatePhysics(true);
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AABBoom::Boom, BoomTime, false);
}

// Called every frame
void AABBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABBoom::SetProperty(TWeakObjectPtr<AActor> InOwnerMadeMe, int InBoomLineDis)
{
	OwnerMadeMe = InOwnerMadeMe;
	BoomLineDis = InBoomLineDis;
}


void AABBoom::Boom()
{
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	// 서버에서만 체크

	if (HasAuthority())
	{
		const float InRate = 0.1f;
		GetWorld()->GetTimerManager().SetTimer(LineBoomTimerHandle, this, &AABBoom::BoomLine, InRate, true, 0.f);
	}
}

void AABBoom::EndBoom()
{
	GetWorld()->GetTimerManager().ClearTimer(LineBoomTimerHandle);
	OnEndBoomDelegate.Broadcast();

	Destroy();
}

void AABBoom::NetMulticastRPC_BoomEffect_Implementation(FVector Location, FVector Scale, bool IsMiddle)
{
	FTransform EffectTransform;
	EffectTransform.SetScale3D(Scale);
	EffectTransform.SetLocation(Location);

	if (IsMiddle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, EffectTransform);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LineParticle, EffectTransform);
	}

}

void AABBoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABBoom, BoomLineDis);

}

void AABBoom::BoomLine()
{
	if (BoomLineCnt > BoomLineDis)
	{
		EndBoom();
		return;
	}

	if (BoomLineCnt == 0)
	{
		FirstLocation = GetActorLocation();
		NetMulticastRPC_BoomEffect(FirstLocation, FVector(1.f, 1.f, 1.f), true);
		Boom(FirstLocation);


		++BoomLineCnt;

		return;
	}

	const float LineDis = 250.f;

	FVector BoomLocation;
	FVector Dir;

	// 좌
	if (EnableDir[0] == false)
	{
		Dir = { 1.f, 0.f, 0.f };
		BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
		if (Boom(BoomLocation))
			EnableDir[0] = true;
	}
	
	// 우
	if (EnableDir[1] == false)
	{
		Dir = { -1.f, 0.f, 0.f };
		BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
		if (Boom(BoomLocation))
			EnableDir[1] = true;
	}

	// 아
	if (EnableDir[2] == false)
	{
		Dir = { 0.f, 1.f, 0.f };
		BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
		if (Boom(BoomLocation))
			EnableDir[2] = true;
	}

	// 위
	if (EnableDir[3] == false)
	{
		Dir = { 0.f, -1.f, 0.f };
		BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
		if (Boom(BoomLocation))
			EnableDir[3] = true;
	}

	++BoomLineCnt;
}

bool AABBoom::Boom(FVector BoomLocation)
{
	NetMulticastRPC_BoomEffect(BoomLocation, FVector(2.f, 2.f, 2.f), false);

	bool IsBreakableHit = false;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, nullptr);

	TArray<FOverlapResult> HitActors;
	bool HitDetected = GetWorld()->OverlapMultiByChannel(HitActors, BoomLocation, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		for (int32 i = 0; i < HitActors.Num(); ++i)
		{
			AActor* HitActor = HitActors[i].GetActor();

			for (int32 t = 0; t < HitActor->Tags.Num(); ++t)
			{
				if (TEXT("Breakable") == HitActor->Tags[t])
				{
					IsBreakableHit = true;
					break;
				}
			}
				

			FDamageEvent DamageEvent;
			HitActor->TakeDamage(10, DamageEvent, nullptr, this);

		}

	}

#if ENABLE_DRAW_DEBUG
	// DrawDebugSphere(GetWorld(), BoomLocation, AttackRadius, 12, FColor::Cyan, false, 5.0f);
#endif

	return IsBreakableHit;
}
