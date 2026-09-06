#include "Skill/SkillBaseComponent.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillData.h"
#include "Skill/EnemySkillData.h"
#include "SkillNames.h"
#include "Character/Player/PlayerCharacter.h"

// 액터 초기화시 각 스킬과 스킬레벨이 0으로 초기화
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

// FName 타입으로 각 스킬명을 구별하여 호출
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
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (!IsValid(Player))
	{
		return;
	}
	if (SkillName == SkillNames::SkillQ)
	{
		Player->PlusSkillQLevel(SkillLevels[SkillName]);
	}
	else if (SkillName == SkillNames::SkillE)
	{
		Player->PlusSkillELevel(SkillLevels[SkillName]);
	}
	else if (SkillName == SkillNames::SkillR)
	{
		Player->PlusSkillRLevel(SkillLevels[SkillName]);
	}
}
