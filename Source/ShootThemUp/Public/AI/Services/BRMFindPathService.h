// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BRMFindPathService.generated.h"

UCLASS()
class SHOOTTHEMUP_API UBRMFindPathService : public UBTService
{
    GENERATED_BODY()

public:
    UBRMFindPathService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector TargetPointActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName CurrentTargetPointIndexKeyName = "CurrentTargetPointIndex";

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
