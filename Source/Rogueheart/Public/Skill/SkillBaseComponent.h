#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUEHEART_API USkillBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillBaseComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 후에 매개변수 추가할 것.
	void UseSkill(FName SkillName);

private:
	TMap<FName, class ASkillBase*> SkillSlot;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* DataTable;
};
