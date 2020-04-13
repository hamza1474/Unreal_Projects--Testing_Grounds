// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrolComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	auto AIControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!AIControlledPawn) { return EBTNodeResult::Failed; }

	auto PatrolComponent = AIControlledPawn->FindComponentByClass<UPatrolComponent>();
	
	if (!ensure(PatrolComponent)) { return EBTNodeResult::Failed; }
	
	PatrolPoints = PatrolComponent->GetPatrolPoints();

	if(PatrolPoints.Num() == 0) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("No Patrol Points detected on [%s]"), *AIControlledPawn->GetName())
		return EBTNodeResult::Failed; 
	}

	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}