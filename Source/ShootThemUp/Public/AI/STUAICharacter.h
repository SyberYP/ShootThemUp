// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "Engine/TargetPoint.h"
#include "STUCoreTypes.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;
class ATargetPoint;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

public:
    ASTUAICharacter(const FObjectInitializer& ObjInit);
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    virtual void Tick(float DeltaTime) override;

    void SetSpawnData(FSpawnData Data) { SpawnData = Data; }
    FSpawnData GetSpawnData() const { return SpawnData; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthVisibilityDistance = 1000.0f;

    virtual void BeginPlay() override;

    virtual void OnDeath() override;
    virtual void OnHealthChanged(float Health, float HealthDelta) override;

    private:
    void UpdateHealthWidgetVisibility();

    FSpawnData SpawnData;

    TArray<ATargetPoint*> TargetPointArray;
};
