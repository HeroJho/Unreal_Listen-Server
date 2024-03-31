// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


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

void AABBox::BreakBox()
{
	FTransform EffectTransform;
	EffectTransform.SetScale3D({ 2.f, 2.f, 2.f });
	EffectTransform.SetLocation(GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BreakParticle, EffectTransform);

	Destroy();
}

