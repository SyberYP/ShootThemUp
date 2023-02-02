// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "UObject/SoftObjectPtr.h"
#include "STUGameInstance.generated.h"

class USoundClass;
class UWorld;
class IOnlineSubsystem;
class FOnlineSessionSettings;

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    FLevelData GetStartupLevel() const { return StartupLevel; }
    void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    TArray<FLevelData> GetLevelsData() const { return LevelsData; }

    FName GetMenuLevelName() const { return MenuLevelName; }

    void ToggleVolume();
    bool HostSession(FName SessionName, int32 MaxPlayers);
    void OpenLevel(FName LevelName);

    virtual void Init() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundClass* MasterSoundClass;

private:
    FLevelData StartupLevel;

    UPROPERTY(EditDefaultsOnly, Category = "Session")
    FName LevelName;

    UPROPERTY(EditDefaultsOnly, Category = "Session")
    FName SessionName;

    // FOnlineSessionSettings SessionSettings;
};
