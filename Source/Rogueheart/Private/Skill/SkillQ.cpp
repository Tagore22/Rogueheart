#include "Skill/SkillQ.h"

void ASkillQ::UseSkill()
{
	Super::UseSkill();

	UE_LOG(LogTemp, Warning, TEXT("Use SkillQ!"));
}

void ASkillQ::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillQ Restored!"));
}

