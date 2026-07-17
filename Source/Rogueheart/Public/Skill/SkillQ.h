#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillQ.generated.h"

UCLASS()
class ROGUEHEART_API ASkillQ : public ASkillBase
{
	GENERATED_BODY()
	
public:
	virtual void UseSkill(class AActor* Target) override;
	
protected:
	virtual void RestoreSkill() override;
};
