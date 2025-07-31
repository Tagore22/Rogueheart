#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SkillType.h"
#include "SkillCooldownWidget.generated.h"

class UProgressBar;

UCLASS()
class ROGUEHEART_API USkillCooldownWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** 바인딩된 델리게이트로부터 호출될 함수 */
    UFUNCTION()
    void OnSkillCooldownUpdated(ESkillType SkillType, float RemainingTime);

    /** 초기화 시 최대 쿨타임 설정 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldown")
    float MaxCooldown = 1.0f;

protected:
    virtual void NativeConstruct() override;

    /** 블루프린트에서 연결될 ProgressBar */
    UPROPERTY(meta = (BindWidget))
    UProgressBar* PB_Cooldown;
};