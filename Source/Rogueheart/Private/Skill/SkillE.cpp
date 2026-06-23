#include "Skill/SkillE.h"

void ASkillE::UseSkill()
{
	Super::UseSkill();

	UE_LOG(LogTemp, Warning, TEXT("Use SkillE!"));
}

void ASkillE::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillE Restored!"));
}