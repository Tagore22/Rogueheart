#include "Skill/SkillBase.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASkillBase::InitializeSkillData(AActor* InitOwn, const FSkillData InitData)
{
	/*if (OwnActor->ActorHasTag(SkillNames::PlayerTag))
	{
		OwnPlayer = Cast<APlayerCharacter>(InitOwn);
	}
	else
	{
		OwnEnemy = Cast<AEnemyBoss>(InitOwn);
	}*/
	OwnActor = Cast<APlayerCharacter>(InitOwn);
	Data = InitData;
}

void ASkillBase::UseSkill(AActor* Target, int32 SkillLevel)
{
	// 여기서 스킬을 실행한다. 이후 오버라이드시 반드시 Super::Activate()를 호출할 것.
	UE_LOG(LogTemp, Warning, TEXT("SkillBase Activate!"));
}

void ASkillBase::RestoreSkill()
{

}