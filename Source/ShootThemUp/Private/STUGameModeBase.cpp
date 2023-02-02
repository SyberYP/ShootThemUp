// Shoot Them Up Game

#include "STUGameModeBase.h"
#include "player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"
#include "UI/STUGameHUD.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "Components/STUWeaponComponent.h"
#include "AI/STUAICharacter.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "STUGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "AI/BRMMobsSpawnerPS.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10.0f;

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    InitPlayerStarts();
    OnOnceSpawn();
   
    GetWorldTimerManager().SetTimer(RespawnMobsTimerHandle, this, &ASTUGameModeBase::OnTimerSpawn, RespawnBotsFrequency, true);
    OnTimerSpawn();
    //SpawnBots();
    CreateTeamsInfoForPlayers();
    //CreateTeamsInfo();
    // RespawnInTeamPlayerStart();

    CurrentRound = 1;
    StartRound();

    SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return BotsPawnMap.FindRef(CurrentBotType);
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (auto PlayerStart : AllPlayerStarts)
    {
        if (!PlayerStart) continue;

        const auto BRMPlayerStart = Cast<ABRMMobsSpawnerPS>(PlayerStart);
        if (!BRMPlayerStart) continue;

        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto BotType = BRMPlayerStart->GetBotType();
        CurrentBotType = BotType;

        //const auto TemplatePawn = Cast<AActor>(BotsPawnMap.FindRef(BotType));
        //DefaultPawnClass = BotsPawnMap.FindRef(BotType);

        //SpawnInfo.Template = TemplatePawn;

        const auto ControllerClass = BotsControllersMap.FindRef(BotType);
        if (!ControllerClass) continue;

        //const auto STUAIController = GetWorld()->SpawnActor<AAIController>(ControllerClass, SpawnInfo);
        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(ControllerClass, SpawnInfo);
        //SpawnActor
        BRMPlayerStart->SpawnMob(Cast<AController>(STUAIController));

        if (BRMPlayerStart->GetTeamID() != 0)
        {
            const auto Controller = Cast<AController>(STUAIController);
            if (!Controller) continue;
            SetTeamAttribute(Controller, BRMPlayerStart->GetTeamID());
        }
    }

    //for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    //{
    //    FActorSpawnParameters SpawnInfo;
    //    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //    const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
    //    // const auto STUAIController = GetWorld()->Spaw

    //    RestartPlayer(STUAIController);
    //    TArray<AActor*> UsedPlayerStarts;
    //    SpawnPlayerAtTeamPlayerStart(UsedPlayerStarts, STUAIController);
    //}
}

void ASTUGameModeBase::SpawnBot(ABRMMobsSpawnerPS* MobsSpawnerPS)
{
    if (!GetWorld()) return;

    if (!MobsSpawnerPS) return;

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const auto BotType = MobsSpawnerPS->GetBotType();
    CurrentBotType = BotType;

    const auto ControllerClass = BotsControllersMap.FindRef(BotType);
    if (!ControllerClass) return;

    const auto STUAIController = GetWorld()->SpawnActor<AAIController>(ControllerClass, SpawnInfo);
    MobsSpawnerPS->SpawnMob(Cast<AController>(STUAIController));

    if (MobsSpawnerPS->GetTeamID() != 0)
    {
        const auto Controller = Cast<AController>(STUAIController);
        if (!Controller) return;
        SetTeamAttribute(Controller, MobsSpawnerPS->GetTeamID());
    }
}

void ASTUGameModeBase::SpawnAtPlayerStartWithTag(AController* Controller, FName PlayerStartTag)
{
    if (PlayerStartTag.IsNone() || !Controller) return;

    TArray<AActor*> PlayerStarts;

    for (auto PlayerStart : AllPlayerStarts)
    {
        if (!PlayerStart) continue;

        RestartPlayerAtPlayerStart(Controller, PlayerStart);
    }
}

void ASTUGameModeBase::RespawnInTeamPlayerStart()
{
    TArray<AActor*> PlayerStarts;
    TArray<AActor*> UsedPlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState || PlayerStarts.Num() == 0)
        {
            continue;
        }

        for (auto PlayerStart : PlayerStarts)
        {
            const auto CurrentPlayerStart = Cast<APlayerStart>(PlayerStart);
            if (!CurrentPlayerStart->PlayerStartTag.IsNone())
            {
                // UE_LOG(LogSTUGameModeBase, Warning, TEXT("Player start with Tag: %s, Team id: %i"), //
                //     *CurrentPlayerStart->PlayerStartTag.ToString(),                                 //
                //     PlayerState->GetTeamID());
            }
            else
            {
                continue;
            }

            int32 team1 = 1;
            int32 team2 = 2;
            FName Team1Name = "Team 1";
            FName Team2Name = "Team 2";
            if (UsedPlayerStarts.Find(CurrentPlayerStart) >= 0) continue;

            if (CurrentPlayerStart->PlayerStartTag == Team1Name && PlayerState->GetTeamID() == team1)
            {
                UE_LOG(LogSTUGameModeBase, Warning, TEXT("Player start with Tag: %s, Team id: %i"), //
                    *CurrentPlayerStart->PlayerStartTag.ToString(),                                 //
                    PlayerState->GetTeamID());
                RestartPlayerAtPlayerStart(Controller, CurrentPlayerStart);
                UsedPlayerStarts.Add(CurrentPlayerStart);
                break;
            }
            else if (CurrentPlayerStart->PlayerStartTag == Team2Name && PlayerState->GetTeamID() == team2)
            {
                UE_LOG(LogSTUGameModeBase, Warning, TEXT("Player start with Tag: %s, Team id: %i"), //
                    *CurrentPlayerStart->PlayerStartTag.ToString(),                                 //
                    PlayerState->GetTeamID());
                RestartPlayerAtPlayerStart(Controller, CurrentPlayerStart);
                UsedPlayerStarts.Add(CurrentPlayerStart);
                break;
            }
        }
    }
}

void ASTUGameModeBase::SpawnPlayerAtTeamPlayerStart(TArray<AActor*>& UsedPlayerStarts, AController* Controller)
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState || PlayerStarts.Num() == 0)
    {
        return;
    }

    for (auto PlayerStart : PlayerStarts)
    {
        const auto CurrentPlayerStart = Cast<APlayerStart>(PlayerStart);
        if (!CurrentPlayerStart->PlayerStartTag.IsNone())
        {
            // UE_LOG(LogSTUGameModeBase, Warning, TEXT("Player start with Tag: %s, Team id: %i"), //
            //     *CurrentPlayerStart->PlayerStartTag.ToString(),                                 //
            //     PlayerState->GetTeamID());
        }
        else
        {
            continue;
        }

        int32 team1 = 1;
        int32 team2 = 2;
        FName Team1Name = "Team 1";
        FName Team2Name = "Team 2";
        if (UsedPlayerStarts.Find(CurrentPlayerStart) >= 0) continue;

        if (CurrentPlayerStart->PlayerStartTag == Team1Name && PlayerState->GetTeamID() == team1)
        {
            UE_LOG(LogSTUGameModeBase, Warning, TEXT("Player start with Tag: %s, Team id: %i"), //
                *CurrentPlayerStart->PlayerStartTag.ToString(),                                 //
                PlayerState->GetTeamID());
            RestartPlayerAtPlayerStart(Controller, CurrentPlayerStart);
            UsedPlayerStarts.Add(CurrentPlayerStart);
            break;
        }
        else if (CurrentPlayerStart->PlayerStartTag == Team2Name && PlayerState->GetTeamID() == team2)
        {
            UE_LOG(LogSTUGameModeBase, Warning, TEXT("Player start with Tag: %s, Team id: %i"), //
                *CurrentPlayerStart->PlayerStartTag.ToString(),                                 //
                PlayerState->GetTeamID());
            RestartPlayerAtPlayerStart(Controller, CurrentPlayerStart);
            UsedPlayerStarts.Add(CurrentPlayerStart);
            break;
        }
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    // UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        SetTeamAttribute(Controller, TeamID);
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

void ASTUGameModeBase::CreateTeamsInfoForPlayers()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller || !Controller->IsPlayerController()) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        SetTeamAttribute(Controller, TeamID);
        PlayerState->SetTeamColor(FLinearColor::Blue);
        SetPlayerColor(Controller);
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

void ASTUGameModeBase::SetTeamAttribute(AController* Controller, int32 TeamID)
{
    if (!Controller) return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);

    if (!PlayerState) return;

    const auto STUPlayerState = Cast<ASTUPlayerState>(PlayerState);
    if (!STUPlayerState) return;

    STUPlayerState->SetTeamID(TeamID);
    STUPlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
    STUPlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
    SetPlayerColor(Controller);
    return;
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if (!Character) return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }
    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }

    StartRespawn(VictimController);
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->LogInfo();
    }
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvaible = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!RespawnAvaible) return;

    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if (!RespawnComponent || !RespawnComponent->GetIsRespawn()) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("======GAME OVER======"));
    LogPlayerInfo();
    GetWorldTimerManager().ClearTimer(RespawnMobsTimerHandle);

    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

    if (PauseSet)
    {
        StopAllFire();
        SetMatchState(ESTUMatchState::Pause);
    }

    return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return PauseCleared;
}

void ASTUGameModeBase::StopAllFire()
{
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Pawn);
        if (!WeaponComponent) continue;

        WeaponComponent->StopFire();
        WeaponComponent->Zoom(false);
    }
}

void ASTUGameModeBase::InitPlayerStarts()
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABRMMobsSpawnerPS::StaticClass(), PlayerStarts);

    for (auto Actor : PlayerStarts)
    {
        const auto Spawner = Cast<ABRMMobsSpawnerPS>(Actor);
        if (Spawner->GetSpawnData().IsTimerSpawn)
        {
            BRMMobsSpawnerPSForTimerSpawn.Add(Spawner);
        }
        else
        {
            BRMMobsSpawnerPSForOnceSpawn.Add(Spawner);
        }
    }

    AllPlayerStarts = PlayerStarts;
    return;
}

void ASTUGameModeBase::OnTimerSpawn()
{
    for (ABRMMobsSpawnerPS* MobsSpawnerPS : BRMMobsSpawnerPSForTimerSpawn)
    {
        if (!MobsSpawnerPS) continue;
        SpawnBot(MobsSpawnerPS);
    }
}

void ASTUGameModeBase::OnOnceSpawn()
{
    for (ABRMMobsSpawnerPS* MobsSpawnerPS : BRMMobsSpawnerPSForOnceSpawn)
    {
        if (!MobsSpawnerPS) continue;
        SpawnBot(MobsSpawnerPS);
    }
}