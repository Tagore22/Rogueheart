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

	UWeaponSweepComponent* SweepComp;

	AActor* OwnerAct = MeshComp->GetOwner();
	if (OwnerAct->ActorHasTag(SkillNames::PlayerTag))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (!IsValid(Player))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is nullptr!"));
			return;
		}
		SweepComp = Player->GetSweepCom();
	}
	else
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(MeshComp->GetOwner());
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

	int32 AttackIndex = 1;

	UWeaponSweepComponent* SweepComp;

	AActor* OwnerAct = MeshComp->GetOwner();
	if (OwnerAct->ActorHasTag(SkillNames::PlayerTag))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (!IsValid(Player))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is nullptr!"));
			return;
		}
		SweepComp = Player->GetSweepCom();
	}
	else
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(MeshComp->GetOwner());
		if (!IsValid(Enemy))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy is nullptr!"));
			return;
		}
		SweepComp = Enemy->GetSweepCom();
		if (Enemy->ActorHasTag(SkillNames::BossTag))
		{
			AttackIndex = Enemy->GetAttackIndex();
		}
	}

	// AttackIndex는 EnemyBase에서 RandRange()를 통해서 0 ~ 공격몽타주.Num() - 1중에서 임의로 만들어진다.
	if (OwnerAct->ActorHasTag(SkillNames::ComboTag))
	{
		SweepComp->SweepAttack(MeshComp->GetSocketLocation(FName(*FString::Printf(TEXT("Weapon_Socket%d"), 0))), 0, true);
		SweepComp->SweepAttack(MeshComp->GetSocketLocation(FName(*FString::Printf(TEXT("Weapon_Socket%d"), 4))), 4, true);
	}
	else
	{
		FString SocketName = FString::Printf(TEXT("Weapon_Socket%d"), AttackIndex);
		SweepComp->SweepAttack(MeshComp->GetSocketLocation(FName(*SocketName)), AttackIndex);
	}
}

void UPlayerAttackANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}