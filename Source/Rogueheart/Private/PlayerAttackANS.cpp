#include "PlayerAttackANS.h"
#include "WeaponSweepComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "SkillNames.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/EnemyAIController.h"

void UPlayerAttackANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh is nullptr!"));
		return;
	}
	AActor* OwnerAct = MeshComp->GetOwner();
	if (!IsValid(OwnerAct))
	{
		return;
	}
	if (OwnerAct->ActorHasTag(SkillNames::PlayerTag))
	{
		Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (!IsValid(Player))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is nullptr!"));
			return;
		}
		SweepComp = Player->GetSweepCom();
	}
	else if (OwnerAct->ActorHasTag(SkillNames::EnemyTag) || OwnerAct->ActorHasTag(SkillNames::DieTag))
	{
		Enemy = Cast<AEnemyBase>(MeshComp->GetOwner());
		if (!IsValid(Enemy))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy is nullptr!"));
			return;
		}
		SweepComp = Enemy->GetSweepCom();
	}
	if (!IsValid(SweepComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sweep is nullptr!"));
		return;
	}
	SweepComp->ClearHitActors();
}

void UPlayerAttackANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh is nullptr!"));
		return;
	}
	if (!IsValid(Player) && !IsValid(Enemy))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is nullptr!"));
		return;
	}
	if (!IsValid(SweepComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sweep is nullptr!"));
		return;
	}
	SweepComp->SweepAttack(MeshComp->GetSocketLocation(TEXT("Weapon_Socket")));
}

void UPlayerAttackANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IsValid(Enemy))
	{
		Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;
		AEnemyAIController* Con = Cast<AEnemyAIController>(Enemy->GetController());
		if (!Con)
		{
			return;
		}
		Con->ToggleBT(false);
	}
}