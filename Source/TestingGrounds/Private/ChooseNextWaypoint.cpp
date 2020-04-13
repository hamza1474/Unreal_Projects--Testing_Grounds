// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrollingGuard.h" // TODO Remove
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	auto AIControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!AIControlledPawn) { return EBTNodeResult::Failed; }

	auto PatrollingGuard = Cast<APatrollingGuard>(AIControlledPawn);
	
	if (!PatrollingGuard) { return EBTNodeResult::Failed; }
	
	PatrolPoints = PatrollingGuard->PatrolPointsCPP;
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}