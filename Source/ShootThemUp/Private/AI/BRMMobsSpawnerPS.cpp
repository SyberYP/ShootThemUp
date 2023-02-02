// Shoot Them Up Game


#include "AI/BRMMobsSpawnerPS.h"
#include "STUGameModeBase.h"
#include "AI/STUAICharacter.h"
#include "AIController.h"

void ABRMMobsSpawnerPS::SpawnMob_Implementation(AController* Controller)
{
    if (!GetWorld()) return;

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->RestartPlayerAtPlayerStart(Controller, this);

    const auto Character = Cast<ASTUAICharacter>(Controller->GetPawn());
    if (!Character) return;

    Character->SetSpawnData(SpawnData);
}