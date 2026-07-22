#include "Character/Player/PlayerSkillRANS.h"
#include "Character/Player/PlayerCharacter.h"

void UPlayerSkillRANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 여기서 검기를 발사한다.
	// UGhostTrailANS과 동일하다.
}

void UPlayerSkillRANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UPlayerSkillRANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());

	Player->SetPlayerState(EPlayerState::Idle);
}

