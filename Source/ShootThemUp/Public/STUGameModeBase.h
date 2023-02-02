// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "AI/BRMMobsSpawnerPS.h"
#include "STUGameModeBase.generated.h"

class AAIController;
class ABRMMobsSpawnerPS;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASTUGameModeBase();

    FOnMatchStateChangedSignature OnMatchStateChanged;

    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    void Killed(AController* KillerController, AController* VictimController);

    FGameData GetGameData() const { return GameData; };
    int32 GetCurrentRoundNum() const { return CurrentRound; };
    int32 GetRoundSecondsRemaining() const { return RoundCountDown; };
    EBRMBotsType CurrentBotType;

    void RespawnRequest(AController* Controller);

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;

    virtual bool ClearPause() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TMap<EBRMBotsType, TSubclassOf<AAIController>> BotsControllersMap;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TMap<EBRMBotsType, TSubclassOf<APawn>> BotsPawnMap;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    float RespawnBotsFrequency = 45.0f; //seconds

private:
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;

    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;
    FTimerHandle RespawnMobsTimerHandle;
    TArray<AActor*> AllPlayerStarts;
    TArray<ABRMMobsSpawnerPS*> BRMMobsSpawnerPSForTimerSpawn;
    TArray<ABRMMobsSpawnerPS*> BRMMobsSpawnerPSForOnceSpawn;

    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(AController* Controller);

    void LogPlayerInfo();

    void StartRespawn(AController* Controller);

    void GameOver();

    void SetMatchState(ESTUMatchState State);

    void StopAllFire();

    void RespawnInTeamPlayerStart();

    void SpawnPlayerAtTeamPlayerStart(TArray<AActor*>& UsedPlayerStarts, AController* Controller);

    void SpawnTeamWaveMobs(int32 TeamID, EBRMBotsType BotType, EBRMSide VerticalSide, EBRMSide HorizontalSide);

    void SetTeamAttribute(AController* Controller, int32 TeamID);

    void SpawnAtPlayerStartWithTag(AController* Controller, FName PlayerStartTag);

    void CreateTeamsInfoForPlayers();

    void OnTimerSpawn();

    void OnOnceSpawn();

    void InitPlayerStarts();

    void SpawnBot(ABRMMobsSpawnerPS* MobsSpawnerPS);
};
