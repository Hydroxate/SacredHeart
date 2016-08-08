// Fill out your copyright notice in the Description page of Project Settings.

#include "SacredHeart.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyAI.h"
#include "SacredHeartCharacter.h"
#include "EnemyCharacter.h"
#include "BTTask_MoveToPlayer.h"

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAI *CharPC = Cast<AEnemyAI>(OwnerComp.GetAIOwner());

	ASacredHeartCharacter *Enemy = Cast<ASacredHeartCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->EnemyKeyID));
	if (Enemy)
	{
		int bpm = Enemy->heartRate;

		if (bpm > 100)
		{
		CharPC->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 550;
		CharPC->MoveToActor(Enemy, 5.f, true, true, true, 0, true);
		return EBTNodeResult::Succeeded;
		}
		else
		{

			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	
}


