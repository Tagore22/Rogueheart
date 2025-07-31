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
    /** ���ε��� ��������Ʈ�κ��� ȣ��� �Լ� */
    UFUNCTION()
    void OnSkillCooldownUpdated(ESkillType SkillType, float RemainingTime);

    /** �ʱ�ȭ �� �ִ� ��Ÿ�� ���� */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldown")
    float MaxCooldown = 1.0f;

protected:
    virtual void NativeConstruct() override;

    /** �������Ʈ���� ����� ProgressBar */
    UPROPERTY(meta = (BindWidget))
    UProgressBar* PB_Cooldown;
};