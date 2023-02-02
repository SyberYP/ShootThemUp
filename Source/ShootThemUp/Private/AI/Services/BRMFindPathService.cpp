// Shoot Them Up Game


#include "AI/Services/BRMFindPathService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"
#include "AI/STUAICharacter.h"
#include "Engine/TargetPoint.h"

UBRMFindPathService::UBRMFindPathService()
{
    NodeName = "Find Next Target Point";
}

void UBRMFindPathService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    //const auto Blackboard = OwnerComp.GetBlackboardComponent();
    //if (Blackboard)
    //{
    //    Blackboard->SetValueAsInt(TargetPointActorKey.SelectedKeyName, 0);
    //    const auto Controller = OwnerComp.GetAIOwner();
    //    //if (!Controller) return;

    //    const auto Character = Controller->GetPawn();
    //    //if (!Character) return;

    //    const auto STUAICharacter = Cast<ASTUAICharacter>(Character);
    //    //if (!STUAICharacter) return;

    //    TArray <ATargetPoint*> TargetPointArray = STUAICharacter->GetTargetArray();
    //    if (TargetPointArray.IsEmpty()) return;

    //    int32 CurrentTargetPointIndex = Blackboard->GetValueAsInt(CurrentTargetPointIndexKeyName);
    //    if (!TargetPointArray.IsValidIndex(CurrentTargetPointIndex)) CurrentTargetPointIndex = 0;
    //    if (!TargetPointArray.IsValidIndex(CurrentTargetPointIndex)) return;

    //    if (TargetPointArray.IsValidIndex(CurrentTargetPointIndex + 1))
    //    {
    //        FVector DistanceToTarget = STUAICharacter->GetActorLocation() - TargetPointArray[CurrentTargetPointIndex]->GetActorLocation();

    //        if (DistanceToTarget.IsNearlyZero(50.0f))
    //        {
    //            CurrentTargetPointIndex += 1;
    //            Blackboard->SetValueAsVector(TargetPointActorKey.SelectedKeyName, TargetPointArray[CurrentTargetPointIndex]->GetActorLocation());
    //            Blackboard->SetValueAsInt(TargetPointActorKey.SelectedKeyName, CurrentTargetPointIndex);
    //        }
    //    }
    //    else
    //    {
    //        Blackboard->SetValueAsVector(
    //            TargetPointActorKey.SelectedKeyName, TargetPointArray[CurrentTargetPointIndex]->GetActorLocation());
    //    }
    //}
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}