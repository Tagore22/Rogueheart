#include "UI/LevelupWidget.h"
#include "Components/Button.h"
#include "Character/Player/PlayerCharacter.h"

void ULevelupWidget::NativeConstruct()
{
    Super::NativeConstruct();

    HPUpButton->OnClicked.AddDynamic(this, &ULevelupWidget::OnHPUpClicked);
    MPUpButton->OnClicked.AddDynamic(this, &ULevelupWidget::OnMPUpClicked);
    StaminaUpButton->OnClicked.AddDynamic(this, &ULevelupWidget::OnStaminaUpClicked);
    SkillQUpButton->OnClicked.AddDynamic(this, &ULevelupWidget::OnSkillQUpClicked);
    SkillEUpButton->OnClicked.AddDynamic(this, &ULevelupWidget::OnSkillEUpClicked);
    SkillRUpButton->OnClicked.AddDynamic(this, &ULevelupWidget::OnSkillRUpClicked);

    Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

void ULevelupWidget::OnHPUpClicked()
{
    Player->SetMaxHP(PlusValue);
}

void ULevelupWidget::OnMPUpClicked()
{
    Player->SetMaxMana(PlusValue);
}

void ULevelupWidget::OnStaminaUpClicked()
{
    Player->SetMaxStamina(PlusValue);
}

void ULevelupWidget::OnSkillQUpClicked()
{

}

void ULevelupWidget::OnSkillEUpClicked()
{

}

void ULevelupWidget::OnSkillRUpClicked()
{

}

