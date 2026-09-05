#include "UI/LevelupWidget.h"
#include "Components/Button.h"
#include "Character/Player/PlayerCharacter.h"
#include "SkillNames.h"
#include "Skill/SkillBaseComponent.h"
#include "Components/TextBlock.h"

void ULevelupWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

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
    UE_LOG(LogTemp, Log, TEXT("HP LevelUp"));

    Player->PlusHPLevel();
    Player->SetMaxHP(Player->GetMaxHP() + PlusValue);
}

void ULevelupWidget::OnMPUpClicked()
{
    UE_LOG(LogTemp, Log, TEXT("MP LevelUp"));

    Player->PlusMPLevel();
    Player->SetMaxMana(Player->GetMaxMana() + PlusValue);
}

void ULevelupWidget::OnStaminaUpClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Stamina LevelUp"));

    Player->PlusStaminaLevel();
    Player->SetMaxStamina(Player->GetMaxStamina() + PlusValue);
}

void ULevelupWidget::OnSkillQUpClicked()
{
    UE_LOG(LogTemp, Log, TEXT("SkillQ LevelUp"));

    SkillLevelUp(SkillNames::SkillQ);
}

void ULevelupWidget::OnSkillEUpClicked()
{
    SkillLevelUp(SkillNames::SkillE);
}

void ULevelupWidget::OnSkillRUpClicked()
{
    SkillLevelUp(SkillNames::SkillR);
}

void ULevelupWidget::SkillLevelUp(FName SkillName)
{
    USkillBaseComponent* SkillBaseCom = Cast<USkillBaseComponent>(Player->GetSkillBaseCom());
    if (!SkillBaseCom)
    {
        return;
    }
    SkillBaseCom->SkillLevelUp(SkillName);
}

void ULevelupWidget::SetTextHPLevel(int32 Num)
{
    HPLevel->SetText(FText::AsNumber(Num));
}

void ULevelupWidget::SetTextMPLevel(int32 Num)
{
    MPLevel->SetText(FText::AsNumber(Num));
}

void ULevelupWidget::SetTextStaminaLevel(int32 Num)
{
    StaminaLevel->SetText(FText::AsNumber(Num));
}

void ULevelupWidget::SetTextSkillQLevel(int32 Num)
{
    SkillQLevel->SetText(FText::AsNumber(Num));
}

void ULevelupWidget::SetTextSkillELevel(int32 Num)
{
    SkillELevel->SetText(FText::AsNumber(Num));
}

void ULevelupWidget::SetTextSkillRLevel(int32 Num)
{
    SkillRLevel->SetText(FText::AsNumber(Num));
}
