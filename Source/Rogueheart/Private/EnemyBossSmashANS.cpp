#include "EnemyBossSmashANS.h"
#include "Character/Enemy/EnemyBoss.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyBossSmashANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AEnemyBoss* OwnActor = Cast<AEnemyBoss>(MeshComp->GetOwner());
	if (!IsValid(OwnActor))
	{
		return;
	}

	MoveCom = OwnActor->GetCharacterMovement();
	if (!MoveCom)
	{
		return;
	}

	MoveCom->GravityScale = 0.f;
	MoveCom->SetMovementMode(MOVE_Falling);
}

void UEnemyBossSmashANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UEnemyBossSmashANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AEnemyBoss* OwnActor = Cast<AEnemyBoss>(MeshComp->GetOwner());
	if (!IsValid(OwnActor))
	{
		return;
	}

	MoveCom = OwnActor->GetCharacterMovement();
	if (!MoveCom)
	{
		return;
	}

	MoveCom->GravityScale = 1.f;
	MoveCom->SetMovementMode(MOVE_Walking);
}

