#include "BTTask/BTTask_Shadowstep.h"
#include "Character/Enemy/EnemyBoss.h"
#include "AIController.h"
#include "SkillNames.h"

EBTNodeResult::Type UBTTask_Shadowstep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBoss* Me = Cast<AEnemyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Me))
	{
		return EBTNodeResult::Failed;
	}

	Me->UseSkill(SkillNames::SkillQ);
	return EBTNodeResult::Succeeded;
}

