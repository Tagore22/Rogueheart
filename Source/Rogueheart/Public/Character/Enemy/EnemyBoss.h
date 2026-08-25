#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "EnemyBoss.generated.h"

UCLASS()
class ROGUEHEART_API AEnemyBoss : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyBoss();

	void UseSkill();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ResetDamageSum() override;

protected:
	virtual void BeginPlay() override;

	virtual void EnemyDie() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "SkillBaseComponent")
	class USkillBaseComponent* SkillBaseCom;

	UPROPERTY()
	class UEnemyHPBarWidget* BossHPBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UEnemyHPBarWidget> WBP_BossHPBar;

	bool bUseShadowstep = false;
	
};
