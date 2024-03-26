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
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	Collision->SetCollisionProfileName(TEXT("PhysicsActor"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void AABBoom::BeginPlay()
{
	Super::BeginPlay();
	
	BoomTime = 5.f;
	AttackRadius = 200.f;

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

void AABBoom::SetProperty(TWeakObjectPtr<AActor> _OwnerMadeMe, int _BoomLineDis)
{
	OwnerMadeMe = _OwnerMadeMe;
	BoomLineDis = _BoomLineDis;
}


void AABBoom::NetMulticastRPC_Boom_Implementation()
{
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	// 서버에서만 체크

	const float InRate = 0.1f;
	GetWorld()->GetTimerManager().SetTimer(LineBoomTimerHandle, this, &AABBoom::BoomLine, InRate, true, 0.f);

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
		GetWorld()->GetTimerManager().ClearTimer(LineBoomTimerHandle);
		Destroy();
		return;
	}

	if (BoomLineCnt == 0)
	{
		FirstLocation = GetActorLocation();
		Boom(FirstLocation);

		++BoomLineCnt;

		return;
	}

	const float LineDis = 250.f;

	FVector BoomLocation;
	FVector Dir;

	// 좌
	Dir = { 1.f, 0.f, 0.f };
	BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
	Boom(BoomLocation);

	// 우
	Dir = { -1.f, 0.f, 0.f };
	BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
	Boom(BoomLocation);

	// 아
	Dir = { 0.f, 1.f, 0.f };
	BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
	Boom(BoomLocation);

	// 위
	Dir = { 0.f, -1.f, 0.f };
	BoomLocation = FirstLocation + Dir * LineDis * float(BoomLineCnt);
	Boom(BoomLocation);

	++BoomLineCnt;
}

void AABBoom::Boom(FVector BoomLocation)
{
	// 클라 이팩트
	BoomEffect(BoomLocation);

	// 서버만 충돌 검사
	if (HasAuthority())
	{
		// const AActor* OwnerMe = GetOwnerMadeMe().Get();
		// FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, OwnerMe);
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, nullptr);

		TArray<FOverlapResult> HitActors;
		bool HitDetected = GetWorld()->OverlapMultiByChannel(HitActors, BoomLocation, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

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
		DrawDebugSphere(GetWorld(), BoomLocation, AttackRadius, 12, FColor::Cyan, false, 5.0f);
#endif
	}

}

void AABBoom::BoomEffect(FVector BoomLocation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, BoomLocation);
}

