// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "BRM_TargetPointNearFinishEQTest.generated.h"

UCLASS()
class SHOOTTHEMUP_API UBRM_TargetPointNearFinishEQTest : public UEnvQueryTest
{
	GENERATED_BODY()

public:
    UBRM_TargetPointNearFinishEQTest(const FObjectInitializer& ObjectInitializer);
    virtual void RunTest(FEnvQueryInstance& QueryInstance) const;
};
