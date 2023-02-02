// Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUMenuHUD.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;
};
