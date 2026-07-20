#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/SkillData.h"
#include "TimerManager.h"
#include "Character/Player/PlayerCharacter.h"
#include "SkillBase.generated.h"

UCLASS()
class ROGUEHEART_API ASkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillBase();

	virtual void InitializeSkillData(APlayerCharacter* InitOwn, const FSkillData InitData);

	virtual void UseSkill(class AActor* Target);
protected:
	virtual void RestoreSkill(); // 쿨타임 다 되었을 때 발동.
protected:
	UPROPERTY()
	class APlayerCharacter* OwnActor = nullptr;

	FSkillData Data;

	FTimerHandle SkillTimer;
};
