// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTURespawnComponent();

    void Respawn(int32 RespawnTime);
    int32 GetRespawnCountDown() const { return RespawnCountDown; }
    bool IsRespawnInProgress() const;
    bool GetIsRespawn() const { return IsRespawn; }

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    bool IsRespawn;

private:
    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountDown = 0;

    void RespawnTimerUpdate();
};
