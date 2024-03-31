// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItem.generated.h"

UCLASS()
class ARENABATTLE_API AABItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


public:


protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);


// Components
protected:
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class USphereComponent> Collision;
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UParticleSystem> Particle;

// ItemData
protected:
	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> ItemData;

};
