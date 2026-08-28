#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillQ.generated.h"

UCLASS()
class ROGUEHEART_API ASkillQ : public ASkillBase
{
	GENERATED_BODY()
	
public:
	virtual bool TryUseSkill(class AActor* Target, int32 SkillLevel) override;
	
protected:
	virtual void ExecuteSkill(class AActor* Target, int32 SkillLevel) override;

	virtual void RestoreSkill(class AActor* Target, int32 SkillLevel) override;

	TArray<UMaterialInterface*> Materials;
};
