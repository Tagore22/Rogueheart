#include "UIManager.h"
#include "SkillCooldownWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void UUIManager::Initialize(APlayerController* InPC)
{
    UE_LOG(LogTemp, Warning, TEXT(">>> UUIManager::Initialize() called"));
    OwningPC = InPC;

    if (WB_SkillCooldownClass)
    {
        SkillCooldownWidget = CreateWidget<USkillCooldownWidget>(OwningPC.Get(), WB_SkillCooldownClass);
        UE_LOG(LogTemp, Warning, TEXT(">>> Creating SkillCooldownWidget: %s"), *GetNameSafe(SkillCooldownWidget));
        if (SkillCooldownWidget)
        {
            SkillCooldownWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT(">>> SkillCooldownWidget added to viewport"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(">>> WB_SkillCooldownClass is INVALID"));  // ภฬ มู
    }
}

USkillCooldownWidget* UUIManager::GetSkillCooldownWidget()
{
    return SkillCooldownWidget;
}