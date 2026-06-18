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

void ASkillBase::Initialize(AActor* InitOwn, const FSkillData InitData)
{
	OwnActor = InitOwn;
	Data = InitData;
}

void ASkillBase::Activate()
{
	// 여기서 스킬을 실행한다. 이후 오버라이드시 반드시 Super::Activate()를 호출할 것.
}

