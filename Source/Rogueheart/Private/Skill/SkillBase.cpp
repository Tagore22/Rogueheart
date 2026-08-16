#include "Skill/SkillBase.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASkillBase::InitializeSkillData(AActor* InitOwn, const FSkillData InitData)
{
	OwnActor = Cast<ACharacter>(InitOwn);
	OwnInterface = OwnActor;
	Data = InitData;
}

void ASkillBase::UseSkill(AActor* Target, int32 SkillLevel)
{
	// 여기서 스킬을 실행한다. 이후 오버라이드시 반드시 Super::Activate()를 호출할 것.
	UE_LOG(LogTemp, Warning, TEXT("SkillBase Activate!"));
}

void ASkillBase::ExecuteSkill(class AActor* Target, int32 SkillLevel)
{

}

void ASkillBase::RestoreSkill(class AActor* Target, int32 SkillLevel)
{

}