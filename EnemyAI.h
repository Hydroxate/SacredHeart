// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAI.generated.h"

UCLASS()
class SACREDHEART_API AEnemyAI : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	class UBlackboardComponent *BlackboardComp;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent *BehaviorTreeComp;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsWalking;

	AEnemyAI();

	virtual void Possess(APawn* InPawn) override;

	uint8 EnemyKeyID;
	
	FName patrolCoordsID;

	FName waitTimeID;
	
};
