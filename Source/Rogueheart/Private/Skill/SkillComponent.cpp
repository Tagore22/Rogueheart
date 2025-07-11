#include "Skill/SkillComponent.h"
#include "GameFramework/Actor.h"
#include "Skill/SkillActor.h"

USkillComponent::USkillComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USkillComponent::BeginPlay()
{
    Super::BeginPlay();

    for (const auto& Elem : Skills)
    {
        CooldownTimers.Add(Elem.Key, 0.0f);
    }
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateCooldowns(DeltaTime);
}

void USkillComponent::UpdateCooldowns(float DeltaTime)
{
    for (auto& Elem : CooldownTimers)
    {
        Elem.Value = FMath::Max(0.0f, Elem.Value - DeltaTime);
    }
}

void USkillComponent::UseSkill(FName SkillName)
{
    UE_LOG(LogTemp, Warning, TEXT("UseSkill")); // 여기 확인할 것.
    if (!Skills.Contains(SkillName)) return;

    FSkillData& Skill = Skills[SkillName];
    float& Cooldown = CooldownTimers.FindOrAdd(SkillName);

    if (Cooldown > 0.0f) return;

    if (Skill.SkillClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();

        UE_LOG(LogTemp, Warning, TEXT("UseSkill"));
        GetWorld()->SpawnActor<ASkillActor>(Skill.SkillClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnParams);
    }

    Cooldown = Skill.Cooldown;
}
