#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillE.generated.h"

UCLASS()
class ROGUEHEART_API ASkillE : public ASkillBase
{
	GENERATED_BODY()

public:
	virtual void UseSkill(class AActor* Target) override;

protected:
	virtual void RestoreSkill() override;
};
