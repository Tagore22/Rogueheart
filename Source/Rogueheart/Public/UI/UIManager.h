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

    // 각 UI 접근 함수 
    USkillCooldownWidget* GetSkillCooldownWidget();

private:
    UPROPERTY()
        TWeakObjectPtr<APlayerController> OwningPC;

    UPROPERTY()
    USkillCooldownWidget* SkillCooldownWidget;

    // 위젯 클래스 에셋 (블루프린트로 지정 예정) 
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USkillCooldownWidget> WB_SkillCooldownClass;
};