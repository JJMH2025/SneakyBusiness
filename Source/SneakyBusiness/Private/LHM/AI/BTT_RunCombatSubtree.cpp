// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_RunCombatSubtree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTT_RunCombatSubtree::UBTT_RunCombatSubtree()
{
	NodeName = TEXT("Run Combat Subtree");
}

EBTNodeResult::Type UBTT_RunCombatSubtree::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    UBehaviorTree* Subtree = Cast<UBehaviorTree>(BB->GetValueAsObject("CombatSubtree"));
    if (!BB || !Subtree)
    {
        return EBTNodeResult::Failed;
    }

    OwnerComp.StartTree(*Subtree, EBTExecutionMode::SingleRun);
    return EBTNodeResult::Succeeded;
}
