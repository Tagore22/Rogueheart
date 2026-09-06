#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUEHEART_API USkillBaseComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	void UseSkill(FName SkillName, class AActor* Target);

	void SkillLevelUp(FName SkillName);

private:
	TMap<FName, class ASkillBase*> SkillSlot;

	TMap<FName, int32> SkillLevels;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* DataTable;
};
