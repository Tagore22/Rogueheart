#include "Skill/SkillQ.h"

void ASkillQ::UseSkill()
{
	Super::UseSkill();

	UE_LOG(LogTemp, Warning, TEXT("Use SkillQ!"));
}

