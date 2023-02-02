// Shoot Them Up Game

#include "AI/Tasks/BRMNextTargetLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/STUAICharacter.h"

UBRMNextTargetLocationTask::UBRMNextTargetLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type UBRMNextTargetLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto AICharacter = Cast<ASTUAICharacter>(Pawn);
    if (!AICharacter) return EBTNodeResult::Failed;

    auto CurrentTargerPointIndex = Blackboard->GetValueAsInt(CurrentTargetPointIndexKey.SelectedKeyName);
    //if (!CurrentTargerPointIndex) return EBTNodeResult::Failed;

    const auto TargetPointArray = AICharacter->GetSpawnData().TargetPointArray;
    if (TargetPointArray.IsEmpty()) return EBTNodeResult::Failed;

    CurrentTargerPointIndex = (CurrentTargerPointIndex + 1) % TargetPointArray.Num();
    //TargetPointArray.IsValid(CurrentTargerPointIndex);
    const auto TargetPoint = TargetPointArray[CurrentTargerPointIndex];

    Blackboard->SetValueAsInt(CurrentTargetPointIndexKey.SelectedKeyName, CurrentTargerPointIndex);
    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, TargetPoint->GetActorLocation());
    return EBTNodeResult::Succeeded;



    /*const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;
    auto Location = Pawn->GetActorLocation();
    if (!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor) return EBTNodeResult::Failed;
        Location = CenterActor->GetActorLocation();
    }

    const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
    if (!Found) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;*/
}
