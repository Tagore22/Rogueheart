#include "Skill/SkillBaseComponent.h"

USkillBaseComponent::USkillBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 후에 매개변수 추가할 것.
void USkillBaseComponent::Activate()
{

}
