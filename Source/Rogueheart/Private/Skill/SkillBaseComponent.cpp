#include "Skill/SkillBaseComponent.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillData.h"

USkillBaseComponent::USkillBaseComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

void USkillBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FSkillData*> SkillList;
	DataTable->GetAllRows<FSkillData>(TEXT("SkillList"), SkillList);

	for (const FSkillData* List : SkillList)
	{
		ASkillBase* NewSkill = NewObject<ASkillBase>(this, List->Type);
		NewSkill->InitializeSkillData(GetOwner(), *List);
		SkillSlot.Add(List->SkillID, NewSkill);
		SkillLevels.Add(List->SkillID, 0);
	}

	UE_LOG(LogTemp, Warning, TEXT("Skill Nums : %d"), SkillSlot.Num());
}

/*void USkillBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}*/

// 후에 매개변수 추가할 것.
void USkillBaseComponent::UseSkill(FName SkillID, AActor* Target)
{
	if (ASkillBase** Skill = SkillSlot.Find(SkillID))
	{
		if (!IsValid(*Skill))
		{
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("UseSkill!"));
		(*Skill)->UseSkill(Target, *SkillLevels.Find(SkillID));
	}
}

void USkillBaseComponent::SkillLevelUp(FName SkillName)
{
	if (ASkillBase** Skill = SkillSlot.Find(SkillName))
	{
		if (!IsValid(*Skill))
		{
			return;
		}
	}
	++SkillLevels[SkillName];
}
