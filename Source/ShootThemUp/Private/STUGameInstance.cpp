// Shoot Them Up Game

#include "STUGameInstance.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Sound/STUSoundFuncLib.h"
// #include "OnlineSubsystemUtils.h"

void USTUGameInstance::ToggleVolume()
{
    USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}

//void USTUGameInstance::Init()
//{
//    Super::Init();
//
//    HostSession(FName("MySession"), 8);
//    OpenLevel(FName("MyOpenWorldLevel"));
//}

void USTUGameInstance::Init()
{
    UWorld* World = GetWorld();
    //IOnlineSubsystem* OnlineSub = GetOnlineSubsystem();
    UWorld* World = GetWorld();
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

    // Create the session settings
    //const auto SessionSettings = FOnlineSessionSettings();
    FOnlineSessionSettings SessionSettings;
    SessionSettings.NumPublicConnections = 8;
    SessionSettings.bIsLANMatch = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.Set(SETTING_MAPNAME, FString("MyOpenWorldLevel"), EOnlineDataAdvertisementType::ViaOnlineService);

    // Create the session
    //OnlineSubsystem->GetSessionInterface()->CreateSession(0, FName("MySession"), SessionSettings);
    HostSession(SessionName, 10);
    // Open the level
    OpenLevel(LevelName);
    //GetWorld()->ServerTravel("MyOpenWorldLevel");
}

bool USTUGameInstance::HostSession(FName SessionName, int32 MaxPlayers)
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid())
        {
            FOnlineSessionSettings SessionSettings;
            SessionSettings.NumPublicConnections = MaxPlayers;
            SessionSettings.bIsLANMatch = true;
            SessionSettings.bShouldAdvertise = true;
            SessionSettings.bUsesPresence = true;
            SessionSettings.Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

            return Sessions->CreateSession(0, SessionName, SessionSettings);
        }
    }
    return false;
}

void USTUGameInstance::OpenLevel(FName LevelName)
{
    UWorld* World = GetWorld();
    if (World)
    {
        World->ServerTravel(LevelName.ToString());
    }
}