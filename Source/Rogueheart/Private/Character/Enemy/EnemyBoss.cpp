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
		SkillBaseCom->UseSkill(SkillNames::SkillQ, UGameplayStatics::GetPlayerCharacter(this, 0));
	}
}

/*void AEnemyBoss::Tick(float DeltaTime)
{
	if (!bUseShadowstep && CurHP <= 50.f)
	{
		// 여기서 애니메이션을 재생하고 그 이후 노타파이로 스킬을 호출.
		UE_LOG(LogTemp, Warning, TEXT("ShadowStep!"));
		UseSkill();
		bUseShadowstep = true;
	}
}*/

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle SkillTimer;
	//GetWorldTimerManager().SetTimer(SkillTimer, this, &AEnemyBoss::UseSkill, 5.f, false);
}

