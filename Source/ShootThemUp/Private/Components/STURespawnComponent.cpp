// Shoot Them Up Game

#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
    if (IsRespawn)
    {
        PrimaryComponentTick.bCanEverTick = true;
    }
    else
    {
        PrimaryComponentTick.bCanEverTick = false;
    }
}

void USTURespawnComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USTURespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld() || !IsRespawn)
        return;
    RespawnCountDown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if (--RespawnCountDown == 0)
    {
        if (!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!GameMode)
            return;
        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}