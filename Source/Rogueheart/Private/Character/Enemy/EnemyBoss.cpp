#include "Character/Enemy/EnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/SkillBaseComponent.h"
#include "UI/EnemyHPBarWidget.h"

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

float AEnemyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActorHasTag(SkillNames::DieTag))
	{
		return ActualDamage;
	}

	// 체력 계산은 이미 부모 클래스에서 하고 내려온 것이기 때문에 UI 갱신만 하면 된다.

	BossHPBarWidget->SetHPPercent(CurHP / MaxHP);
	BossHPBarWidget->SetDamageSum(ActualDamage);
	UE_LOG(LogTemp, Warning, TEXT("BossHP : %f"), CurHP);

	DamageReact();

	return ActualDamage;
}

void AEnemyBoss::ResetDamageSum()
{
	Super::ResetDamageSum();

	if (!BossHPBarWidget)
	{
		return;
	}
	BossHPBarWidget->ResetDamageSum();
}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(SkillNames::BossTag);

	if (WBP_BossHPBar)
	{
		BossHPBarWidget = CreateWidget<UEnemyHPBarWidget>(GetWorld(), WBP_BossHPBar);
		BossHPBarWidget->AddToViewport();
		BossHPBarWidget->SetVisibleDamageText(false);
	}

	//HPBarWidget->SetVisibility(true);

	//FTimerHandle SkillTimer;
	//GetWorldTimerManager().SetTimer(SkillTimer, this, &AEnemyBoss::UseSkill, 5.f, false);
}

void AEnemyBoss::EnemyDie()
{
	Super::EnemyDie();

	if (!BossHPBarWidget)
	{
		return;
	}
	BossHPBarWidget->RemoveFromParent();
}

