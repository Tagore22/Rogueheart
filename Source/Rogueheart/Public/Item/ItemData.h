#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "ItemData.generated.h"

class UNiagaraSystem;

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Consumable,
    Equipment,
    Material,
    KeyItem,
    None
};

UENUM(BlueprintType)
enum class EItemEffectType : uint8
{
    HealHP,
    IncreaseAttack,
    SpeedBuff,
    None
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
    FName ItemID;                                         // 아이템의 고유 키.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
    FText ItemName;                                       // 인벤토리/UI에 표시될 이름.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
    FText Description;                                    // 설명창에 들어갈 텍스트.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
    EItemType ItemType = EItemType::None;                 // 소모품/장비/재료/키아이템 등의 구분.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UTexture2D> Icon;                      // 인벤토리 슬롯등에 표시될 2D 아이콘.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UStaticMesh> PickupMesh;               // 필드에 떨어진 모습.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UNiagaraSystem> PickupParticle = nullptr; // 습득 시 이펙트.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<USkeletalMesh> EquipMesh;              // 장착 시 모습 (필요하면)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stack")
    int32 MaxStack = 1;                                   // 최대 겹침 개수. 포션은 99, 장비는 1

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float EffectValue = 0.f;                              // 효과의 수치. +50 HP, +20% 등.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    EItemEffectType EffectType = EItemEffectType::None;   // 체력회복, 대미지증가, 속도 버프의 구분.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float CooldownSeconds = 0.f;                          // 사용 쿨타임(초). 0이면 쿨타임 없음.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float DurationSeconds = 0.f;                          // 버프/디버프 지속 시간.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float Range = 0.f;                                    // 공격 사거리.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    int32 MinDamage = 0;                                      // 최소 공격력.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    int32 MaxDamage = 0;                                      // 최대 공격력.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FName EquipSocketName = NAME_None;                    // 장착 시 붙일 소켓 이름 (RightHandSocket 등).

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> UseSound = nullptr;        // 사용 시 재생될 사운드.
};