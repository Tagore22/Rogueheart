#include "Skill/SkillE.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASkillE::UseSkill(AActor* Target)
{
	Super::UseSkill(Target);

	UE_LOG(LogTemp, Warning, TEXT("Use SkillE!"));

	bool bCanUseSkill = GetWorldTimerManager().IsTimerActive(SkillTimer);
	if (bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("Remain CoolTime is %f!"), GetWorldTimerManager().GetTimerRemaining(SkillTimer));
		return;
	}
	DefaultSpeed = OwnActor->GetCharacterMovement()->MaxWalkSpeed;
	GetWorldTimerManager().SetTimer(SkillTimer, this, &ASkillE::RestoreSkill, Data.Cooldown, false);
	OwnActor->GetCharacterMovement()->MaxWalkSpeed *= 1.5f;
	OwnActor->GetMesh()->GlobalAnimRateScale = 1.5f;
}

void ASkillE::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillE Restored!"));

	OwnActor->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	OwnActor->GetMesh()->GlobalAnimRateScale = 1.f;
}