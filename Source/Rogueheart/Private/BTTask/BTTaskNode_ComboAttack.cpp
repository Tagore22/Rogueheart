#include "BTTask/BTTaskNode_ComboAttack.h"
#include "Character/Enemy/EnemyBoss.h"
#include "AIController.h"
#include "SkillNames.h"

EBTNodeResult::Type UBTTaskNode_ComboAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBoss* Me = Cast<AEnemyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Me))
	{
		return EBTNodeResult::Failed;
	}

	Me->UseSkill(SkillNames::BossPunchCombo);
	return EBTNodeResult::Succeeded;
}
