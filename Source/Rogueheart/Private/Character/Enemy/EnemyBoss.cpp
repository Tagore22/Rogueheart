#include "Character/Enemy/EnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/SkillBaseComponent.h"

AEnemyBoss::AEnemyBoss()
{
	SkillBaseCom = CreateDefaultSubobject<USkillBaseComponent>(TEXT("SKillBaseComponent"));

}

void AEnemyBoss::UseSkill()
{
	int32 Case = FMath::RandRange(1, 10);

	if (Case >= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy UseSkill!"));
		SkillBaseCom->UseSkill(SkillNames::BossPunchCombo, UGameplayStatics::GetPlayerCharacter(this, 0));
	}
}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(SkillNames::BossTag);

	//HPBarWidget->SetVisibility(true);

	FTimerHandle SkillTimer;
	GetWorldTimerManager().SetTimer(SkillTimer, this, &AEnemyBoss::UseSkill, 5.f, false);
}

