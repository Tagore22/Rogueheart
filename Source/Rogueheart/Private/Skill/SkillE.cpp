#include "Skill/SkillE.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASkillE::UseSkill(AActor* Target, int32 SkillLevel)
{
	Super::UseSkill(Target, SkillLevel);

	UE_LOG(LogTemp, Warning, TEXT("Use SkillE!"));

	bool bCanUseSkill = GetWorldTimerManager().IsTimerActive(SkillTimer);
	float Cost = OwnInterface->GetCurMana();
	if (bCanUseSkill || !Data.Effect || Cost <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Remain CoolTime is %f!"), GetWorldTimerManager().GetTimerRemaining(SkillTimer));
		return;
	}
	DefaultSpeed = OwnActor->GetCharacterMovement()->MaxWalkSpeed;
	OwnInterface->SetEffectCom(Data.Effect); 

	GetWorldTimerManager().SetTimer(SkillTimer, this, &ASkillE::RestoreSkill, Data.Cooldown[SkillLevel], false);

	OwnActor->GetCharacterMovement()->MaxWalkSpeed *= Data.SpeedRatio[SkillLevel];
	OwnActor->GetMesh()->GlobalAnimRateScale = Data.SpeedRatio[SkillLevel];
	OwnInterface->ActivateEffectCom(true); 
	OwnInterface->CostMana(Data.Cost[SkillLevel]);
	UE_LOG(LogTemp, Warning, TEXT("SkillE Level : %d"), SkillLevel);
}

void ASkillE::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillE Restored!"));

	OwnActor->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	OwnActor->GetMesh()->GlobalAnimRateScale = 1.f;
	OwnInterface->ActivateEffectCom(false); 
}