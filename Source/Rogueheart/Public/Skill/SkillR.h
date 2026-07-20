#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillR.generated.h"

UCLASS()
class ROGUEHEART_API ASkillR : public ASkillBase
{
	GENERATED_BODY()
	
public:
	virtual void UseSkill(class AActor* Target) override;

protected:
	virtual void RestoreSkill() override;

	void SpawnDummy();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* BladeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float SpawnTime = 0.5f;

	int32 SpawnNumber = 0;
	
	FTimerHandle SpawnTimer;
};
