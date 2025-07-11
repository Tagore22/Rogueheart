#include "Skill/SkillActor.h"

ASkillActor::ASkillActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASkillActor::BeginPlay()
{
    Super::BeginPlay();
}

void ASkillActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASkillActor::ActivateSkill()
{
    // 스킬 발동시 동작 (예: 이펙트, 데미지 등)
}