#pragma once

#include "UObject/SoftObjectPtr.h"
#include "STUCoreTypes.generated.h"

// weapon
class ASTUBaseWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeapon*);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrosshairIcon;
};

// healh
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);


//VFX

class UNiagaraSystem;
class USoundCue;

USTRUCT(BlueprintType) 
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "VFX")
    UMaterialInterface* Material;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(10.0f);

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f;
};

USTRUCT(BlueprintType) 
struct FImpactData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "VFX")
    USoundCue* Sound;
};

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
    int32 PlayersNum = 2;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
    int32 RoundsNum = 4;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "1000"))
    int32 RoundTime = 60; // in seconds

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite)
    FLinearColor DefaultTeamColor = FLinearColor::Red;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite)
    TArray<FLinearColor> TeamColors;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
    int32 RespawnTime = 5; // in seconds
};

UENUM(BlueprintType)
enum class ESTUMatchState : uint8
{
    WaitingToStart = 0,
    InProgress = 1,
    Pause = 2,
    GameOver = 3
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESTUMatchState);

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game")
    FName LevelDisplayName = NAME_None;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelThumb;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Game")
    TSoftObjectPtr<UWorld> LevelReference;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

UENUM(BlueprintType)
enum class EBRMBotsType : uint8
{
    Melee = 0,
    Range = 1,
    Neutral = 2,
    Merchant = 3
};

UENUM(BlueprintType)
enum class EBRMSide : uint8
{
    Top = 0,
    Bottom = 1,
    Left = 2,
    Right = 3,
    Middle = 4,
    Specific = 5
};

USTRUCT(BlueprintType)
struct FSpawnData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditAnywhere, Category = "Mob")
    EBRMBotsType BotType = EBRMBotsType::Neutral;

    UPROPERTY(EditAnywhere, Category = "Mob")
    EBRMSide BotSide = EBRMSide::Specific;

    UPROPERTY(EditAnywhere, Category = "Mob")
    int32 TeamID = 0; // 0 for neutral

    UPROPERTY(EditAnywhere, Category = "Mob")
    int32 LifeTime = 0; // 0 for infinity

    UPROPERTY(EditAnywhere, Category = "Mob")
    int32 RespawnTime = 45; // only for neutral and merchant mobs

    UPROPERTY(EditAnywhere, Category = "Mob")
    int32 GoldGorKill = 10;

    UPROPERTY(EditAnywhere, Category = "Mob")
    bool IsSpawnActive = true;

    UPROPERTY(EditAnywhere, Category = "Mob")
    bool IsTimerSpawn = true;

    UPROPERTY(EditAnywhere, Category = "Behavior")
    TArray<AActor*> TargetPointArray;
};