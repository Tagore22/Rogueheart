#include "UIManager.h"
#include "SkillCooldownWidget.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SkillComponent.h" 
#include "GameFramework/PlayerController.h"

void UUIManager::Initialize(APlayerController* InPC)
{
    OwningPC = InPC;

    if (WB_SkillCooldownClass)
    {
        SkillCooldownWidget = CreateWidget<USkillCooldownWidget>(OwningPC.Get(), WB_SkillCooldownClass);
        if (SkillCooldownWidget)
        {
            SkillCooldownWidget->AddToViewport();

            // 2) SkillComponent ã�Ƽ� ��������Ʈ ���ε�
            APawn* Pawn = OwningPC->GetPawn();
            if (Pawn)
            {
                if (USkillComponent* SkillComp = Pawn->FindComponentByClass<USkillComponent>())
                {
                    FName SkillName = FName("Fireball");
                    if (FSkillDataRow* Row = SkillComp->SkillDataTable->FindRow<FSkillDataRow>(SkillName, TEXT("")))
                    {
                        SkillCooldownWidget->MaxCooldown = Row->Cooldown;  // ���� MaxCooldown ����
                    }
                    SkillComp->OnSkillCooldownUpdated.AddDynamic(
                        SkillCooldownWidget,
                        &USkillCooldownWidget::OnSkillCooldownUpdated
                    );
                }
            }
        }
    }
}

USkillCooldownWidget* UUIManager::GetSkillCooldownWidget()
{
    return SkillCooldownWidget;
}