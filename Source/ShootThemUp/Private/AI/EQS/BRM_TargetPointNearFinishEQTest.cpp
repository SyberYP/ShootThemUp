// Shoot Them Up Game

#include "AI/EQS/BRM_TargetPointNearFinishEQTest.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Engine/TargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBRM_TargetPointNearFinishEQTest::UBRM_TargetPointNearFinishEQTest(const FObjectInitializer& ObjectInitializer) //
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UBRM_TargetPointNearFinishEQTest::RunTest(FEnvQueryInstance& QueryInstance) const
{
    const auto DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    const bool WantsBeTakable = BoolValue.GetValue();

    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard) return;

    const auto ContextActor = Blackboard->GetValueAsObject("PreviousTargetPoint");

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto TargetPointActor = Cast<ATargetPoint>(ItemActor);
        if (!TargetPointActor && !TargetPointActor->Tags.IsEmpty()) continue;

        It.SetScore(TestPurpose, FilterType, TargetPointActor != ContextActor, WantsBeTakable);
    }
}
