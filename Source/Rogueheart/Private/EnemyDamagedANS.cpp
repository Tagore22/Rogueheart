#include "EnemyDamagedANS.h"
#include "Controller/EnemyAIController.h"
#include "Character/Enemy/EnemyBase.h"

void UEnemyDamagedANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AEnemyBase* Owner = Cast<AEnemyBase>(MeshComp->GetOwner());

	if (!IsValid(Owner))
	{
		return;
	}

	AEnemyAIController* Con = Cast<AEnemyAIController>(Owner->GetController());
	if (!Con)
	{
		return;
	}
	Con->ToggleBT(true);
}

void UEnemyDamagedANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UEnemyDamagedANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AEnemyBase* Owner = Cast<AEnemyBase>(MeshComp->GetOwner());
	
	if (!IsValid(Owner))
	{
		return;
	}

	AEnemyAIController* Con = Cast<AEnemyAIController>(Owner->GetController());
	if (!Con)
	{
		return;
	}
	Con->ToggleBT(false);
}