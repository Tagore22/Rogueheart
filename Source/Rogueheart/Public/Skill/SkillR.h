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
	int32 SpawnNumber = 0;
	
	FTimerHandle SpawnTimer;
};
