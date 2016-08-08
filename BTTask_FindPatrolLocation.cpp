// Fill out your copyright notice in the Description page of Project Settings.

#include "SacredHeart.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BTTask_FindPatrolLocation.h"
#include "SacredHeartCharacter.h"
#include "EnemyCharacter.h"
#include "EnemyAI.h"


EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AEnemyAI* EnemyPC = Cast<AEnemyAI>(OwnerComp.GetAIOwner());
	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(OwnerComp.GetWorld());
	if (NavSys && EnemyPC && EnemyPC->GetPawn())
	{
		const float SearchRadius = 1500.0f;
		FNavLocation RandomPt;
		EnemyPC->patrolCoordsID = "PatrolLocation";
		EnemyPC->waitTimeID = "WaitTime";

		const bool bFound = NavSys->GetRandomPointInNavigableRadius(EnemyPC->GetPawn()->GetActorLocation(), SearchRadius, RandomPt);
		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(EnemyPC->patrolCoordsID, RandomPt.Location);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Patrolling");
			EnemyPC->bIsWalking = true;
			//EnemyPC->GetCharacter()->GetCharacterMovement()->IsWalking() = true;
			
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Float>(EnemyPC->waitTimeID, FMath::RandRange(4.f,12.f));
			EnemyPC->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 200;
			
			return EBTNodeResult::Succeeded;
		}

		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else {

		return EBTNodeResult::Failed;
	}
}

