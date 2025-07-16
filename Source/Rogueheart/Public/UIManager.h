#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

class USkillCooldownWidget;

UCLASS(Blueprintable)
class ROGUEHEART_API UUIManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(class APlayerController* InPC);

    // �� UI ���� �Լ� 
    USkillCooldownWidget* GetSkillCooldownWidget();

private:
    UPROPERTY()
        TWeakObjectPtr<APlayerController> OwningPC;

    UPROPERTY()
    USkillCooldownWidget* SkillCooldownWidget;

    // ���� Ŭ���� ���� (�������Ʈ�� ���� ����) 
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USkillCooldownWidget> WB_SkillCooldownClass;
};