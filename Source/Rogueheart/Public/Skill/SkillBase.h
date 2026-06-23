#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/SkillData.h"
#include "TimerManager.h"
#include "SkillBase.generated.h"

UCLASS()
class ROGUEHEART_API ASkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void InitializeSkillData(AActor* InitOwn, const FSkillData InitData);

	virtual void UseSkill();
protected:
	UPROPERTY()
	class AActor* OwnActor = nullptr;

	FSkillData Data;

	FTimerHandle SkillTimer;

	virtual void RestoreSkill();
};
