#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "BossPunchCombo.generated.h"

UCLASS()
class ROGUEHEART_API ABossPunchCombo : public ASkillBase
{
	GENERATED_BODY()

public:
	virtual void UseSkill(class AActor* Target, int32 SkillLevel) override;

protected:
	virtual void ExecuteSkill(class AActor* Target, int32 SkillLevel) override;

	virtual void RestoreSkill(class AActor* Target, int32 SkillLevel) override;
};
