#include "Skill/SkillBaseComponent.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillData.h"
#include "Skill/EnemySkillData.h"
#include "SkillNames.h"

USkillBaseComponent::USkillBaseComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

void USkillBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	/*AActor* Owner = GetOwner();
	if (Owner->ActorHasTag(SkillNames::PlayerTag))
	{
		TArray<FSkillData*> SkillList;
		LoadSkillData(SkillList);
	}
	else
	{
		TArray<FEnemySkillData*> SkillList;
		LoadSkillData(SkillList);
	}*/

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

/*template<typename T>
void USkillBaseComponent::LoadSkillData(const TArray<T*>& SkillList)
{
	for (const T* List : SkillList)
	{
		ASkillBase* NewSkill = NewObject<ASkillBase>(this, List->Type);
		NewSkill->InitializeSkillData(GetOwner(), *List);
		SkillSlot.Add(List->SkillID, NewSkill);
		SkillLevels.Add(List->SkillID, 0);
	}
}*/

/*void USkillBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}*/

// 후에 매개변수 추가할 것.
void USkillBaseComponent::UseSkill(FName SkillID, AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("UseSkill! Prev"));
	if (ASkillBase** Skill = SkillSlot.Find(SkillID))
	{
		UE_LOG(LogTemp, Warning, TEXT("UseSkill! middle"));
		if (!IsValid(*Skill))
		{
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("UseSkill!"));
		(*Skill)->TryUseSkill(Target, *SkillLevels.Find(SkillID));
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
