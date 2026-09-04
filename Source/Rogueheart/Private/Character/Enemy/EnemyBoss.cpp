#include "Character/Enemy/EnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/SkillBaseComponent.h"
#include "UI/EnemyHPBarWidget.h"
#include "Controller/EnemyAIController.h"

AEnemyBoss::AEnemyBoss()
{
	SkillBaseCom = CreateDefaultSubobject<USkillBaseComponent>(TEXT("SKillBaseComponent"));

}

void AEnemyBoss::UseSkill(FName SkillName)
{
	SkillBaseCom->UseSkill(SkillName, UGameplayStatics::GetPlayerCharacter(this, 0));
}

float AEnemyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActorHasTag(SkillNames::DieTag))
	{
		return ActualDamage;
	}

	// 체력 계산은 이미 부모 클래스에서 하고 내려온 것이기 때문에 UI 갱신만 하면 된다.

	float Percent = CurHP / MaxHP;
	BossHPBarWidget->SetHPPercent(Percent);
	BossHPBarWidget->SetDamageSum(ActualDamage);
	UE_LOG(LogTemp, Warning, TEXT("BossHP : %f"), CurHP);

	DamageReact();

	if (Percent <= 0.5f)
	{
		AEnemyAIController* AIC = Cast<AEnemyAIController>(GetController());
		if (!AIC)
		{
			return ActualDamage;
		}
		AIC->SetbIsPhaseTwo(true);
	}

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

