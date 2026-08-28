#include "Skill/BossPunchCombo.h"

bool ABossPunchCombo::TryUseSkill(AActor* Target, int32 SkillLevel)
{
	Super::TryUseSkill(Target, SkillLevel);

	if (!Data.SkillMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("PunchCombo has no montage"));
		return false;
	}

	OwnActor->Tags.Add(SkillNames::ComboTag);
	ExecuteSkill(Target, SkillLevel);

	return true;
}

void ABossPunchCombo::ExecuteSkill(class AActor* Target, int32 SkillLevel)
{
	Super::ExecuteSkill(Target, SkillLevel);

	UAnimInstance* Anim = OwnActor->GetMesh()->GetAnimInstance();
	if (!Anim)
	{
		return;
	}
	Anim->Montage_Play(Data.SkillMontage);
	float MontageTime = Data.SkillMontage->GetPlayLength();
	TimerDelegate.BindUObject(this, &ABossPunchCombo::RestoreSkill, Target, SkillLevel);
	GetWorldTimerManager().SetTimer(SkillTimer, TimerDelegate, MontageTime - 1.5f, false);
	UE_LOG(LogTemp, Warning, TEXT("AnimTime : %f"), MontageTime);
}

void ABossPunchCombo::RestoreSkill(class AActor* Target, int32 SkillLevel)
{
	Super::RestoreSkill(Target, SkillLevel);

	OwnActor->Tags.Remove(SkillNames::ComboTag);
}
