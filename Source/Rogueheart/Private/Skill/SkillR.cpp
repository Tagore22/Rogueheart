#include "Skill/SkillR.h"

void ASkillR::UseSkill()
{
	Super::UseSkill();

	UE_LOG(LogTemp, Warning, TEXT("Use SkillR!"));
}

void ASkillR::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillR Restored!"));
}
