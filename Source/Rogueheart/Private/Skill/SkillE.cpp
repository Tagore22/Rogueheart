#include "Skill/SkillE.h"
#include "GameFramework/CharacterMovementComponent.h"

bool ASkillE::TryUseSkill(AActor* Target, int32 SkillLevel)
{
	Super::TryUseSkill(Target, SkillLevel);

	UE_LOG(LogTemp, Warning, TEXT("Use SkillE!"));

	bool bCanUseSkill = GetWorldTimerManager().IsTimerActive(SkillTimer);
	float Cost = OwnInterface->GetCurMana();
	if (bCanUseSkill || !Data.Effect || Cost <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Remain CoolTime is %f!"), GetWorldTimerManager().GetTimerRemaining(SkillTimer));
		return false;
	}

	Cooldown = Data.Cooldown[SkillLevel];
	// 후에 이곳에서 조건을 걸 수 있다.

	TimerDelegate.BindUObject(this, &ASkillE::ExecuteSkill, Target, SkillLevel);
	GetWorldTimerManager().SetTimer(SkillTimer, TimerDelegate, Cooldown, false);

	return true;
}

void ASkillE::ExecuteSkill(class AActor* Target, int32 SkillLevel)
{
	Super::ExecuteSkill(Target, SkillLevel);

	TimerDelegate.BindUObject(this, &ASkillE::RestoreSkill, Target, SkillLevel);
	GetWorldTimerManager().SetTimer(SkillTimer, TimerDelegate, Cooldown, false);

	DefaultSpeed = OwnActor->GetCharacterMovement()->MaxWalkSpeed;
	OwnInterface->SetEffectCom(Data.Effect);

	OwnActor->GetCharacterMovement()->MaxWalkSpeed *= Data.SpeedRatio[SkillLevel];
	OwnActor->GetMesh()->GlobalAnimRateScale = Data.SpeedRatio[SkillLevel];
	OwnInterface->ActivateEffectCom(true);
	OwnInterface->CostMana(Data.Cost[SkillLevel]);
	UE_LOG(LogTemp, Warning, TEXT("SkillE Level : %d"), SkillLevel);
}

void ASkillE::RestoreSkill(class AActor* Target, int32 SkillLevel)
{
	Super::RestoreSkill(Target, SkillLevel);

	UE_LOG(LogTemp, Warning, TEXT("SkillE Restored!"));

	OwnActor->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	OwnActor->GetMesh()->GlobalAnimRateScale = 1.f;
	OwnInterface->ActivateEffectCom(false); 
}