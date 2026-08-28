#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillE.generated.h"

UCLASS()
class ROGUEHEART_API ASkillE : public ASkillBase
{
	GENERATED_BODY()

public:
	virtual bool TryUseSkill(class AActor* Target, int32 SkillLevel) override;

protected:
	virtual void ExecuteSkill(class AActor* Target, int32 SkillLevel) override;

	virtual void RestoreSkill(class AActor* Target, int32 SkillLevel) override;

protected:
	float DefaultSpeed; 
};
