#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillR.generated.h"

UCLASS()
class ROGUEHEART_API ASkillR : public ASkillBase
{
	GENERATED_BODY()
	
public:
	virtual bool TryUseSkill(class AActor* Target, int32 SkillLevel) override;

protected:
	virtual void ExecuteSkill(class AActor* Target, int32 SkillLevel) override;

	virtual void RestoreSkill(class AActor* Target, int32 SkillLevel) override;

	void SpawnDummy();
protected:
	int32 SpawnNumber = 0;
	
	int32 RLevel;

	FTimerHandle SpawnTimer;
};
