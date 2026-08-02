#include "Character/Player/GhostTrailANS.h"
#include "Character/Player/PlayerGhostTrail.h"
#include "Skill/BladeGhostTrail.h"

void UGhostTrailANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 여기서 검기를 발사한다.
	// UPlayerSkillRANS과 동일하다.
	APlayerGhostTrail* Act = Cast<APlayerGhostTrail>(MeshComp->GetOwner());
	if (!IsValid(Act))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast Failed!"));
		return;
	}
	FVector ForwardVec = Act->GetActorForwardVector();
	ABladeGhostTrail* GhostTrail = Cast<ABladeGhostTrail>(MeshComp->GetWorld()->SpawnActor<AActor>(Act->GetBlade(), Act->GetActorLocation() + ForwardVec * Act->GetPlusDistance(), Act->GetActorRotation()));
	if (!IsValid(GhostTrail))
	{
		return;
	}
	GhostTrail->InitializeBGT(Act->GetBladeDamage());
	UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
}

void UGhostTrailANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UGhostTrailANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	MeshComp->GetOwner()->SetLifeSpan(0.01f);
}