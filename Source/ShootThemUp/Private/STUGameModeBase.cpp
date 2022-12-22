// Shoot Them Up Game


#include "STUGameModeBase.h"
#include "player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"
#include "UI/STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}