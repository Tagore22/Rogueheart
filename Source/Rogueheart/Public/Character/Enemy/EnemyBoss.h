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

protected:
	//virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "SkillBaseComponent")
	class USkillBaseComponent* SkillBaseCom;

	bool bUseShadowstep = false;
	
};
