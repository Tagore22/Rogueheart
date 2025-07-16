#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCooldownWidget.generated.h"

class UProgressBar;

UCLASS()
class ROGUEHEART_API USkillCooldownWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** ���ε��� ��������Ʈ�κ��� ȣ��� �Լ� */
    UFUNCTION()
        void OnSkillCooldownUpdated(FName SkillName, float RemainingTime);

    /** �ʱ�ȭ �� �ִ� ��Ÿ�� ���� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
        float MaxCooldown = 5.0f;

protected:
    virtual void NativeConstruct() override;

    /** �������Ʈ���� ����� ProgressBar */
    UPROPERTY(meta = (BindWidget))
        UProgressBar* PB_Cooldown;
};