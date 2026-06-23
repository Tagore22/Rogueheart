#include "Skill/SkillBase.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASkillBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillBase::InitializeSkillData(AActor* InitOwn, const FSkillData InitData)
{
	OwnActor = InitOwn;
	Data = InitData;
}

void ASkillBase::UseSkill()
{
	// 여기서 스킬을 실행한다. 이후 오버라이드시 반드시 Super::Activate()를 호출할 것.
	bool bCanUseSkill = GetWorldTimerManager().IsTimerActive(SkillTimer);
	if (bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("Remain CoolTime is %f!"), GetWorldTimerManager().GetTimerRemaining(SkillTimer));
		return;
	}

	GetWorldTimerManager().SetTimer(SkillTimer, this, &ASkillBase::RestoreSkill, Data.Cooldown, false);
	UE_LOG(LogTemp, Warning, TEXT("SkillBase Activate!"));
}

void ASkillBase::RestoreSkill()
{

}
