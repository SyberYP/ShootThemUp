// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "STUCoreTypes.h"
#include "Engine/TargetPoint.h"
#include "BRMMobsSpawnerPS.generated.h"

class ATargetPoint;

UCLASS()
class SHOOTTHEMUP_API ABRMMobsSpawnerPS : public APlayerStart
{
	GENERATED_BODY()

public:
    UFUNCTION(Server, Reliable)
    void SpawnMob(AController* Controller);

    AActor* GetSpawnedActor() const { return SpawnedActorRef; }
    int32 GetTeamID() const { return SpawnData.TeamID; }
    //TSubclassOf<AAIController> GetAIController() const { return AIControllerClass; }
    EBRMBotsType GetBotType() const { return SpawnData.BotType; }
    FSpawnData GetSpawnData() const { return SpawnData; }

protected:
    UPROPERTY(EditAnywhere, Category = "Mob")
    FSpawnData SpawnData;

private:
    AActor* SpawnedActorRef;

};
