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
    // ��ų �ߵ��� ���� (��: ����Ʈ, ������ ��)
}