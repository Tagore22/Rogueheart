#include "PlayerAttackANS.h"
#include "WeaponSweepComponent.h"

void UPlayerAttackANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh is nullptr!"));
		return;
	}
	AActor* Player = MeshComp->GetOwner();
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is nullptr!"));
		return;
	}
	UWeaponSweepComponent* SweepComp = Player->FindComponentByClass<UWeaponSweepComponent>();
	if (!IsValid(SweepComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sweep is nullptr!"));
		return;
	}
	SweepComp->ClearHitActors();
}

void UPlayerAttackANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh is nullptr!"));
		return;
	}
	AActor* Player = MeshComp->GetOwner();
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is nullptr!"));
		return;
	}
	UWeaponSweepComponent* SweepComp = Player->FindComponentByClass<UWeaponSweepComponent>();
	if (!IsValid(SweepComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sweep is nullptr!"));
		return;
	}
	SweepComp->SweepAttack(MeshComp->GetSocketLocation(TEXT("Weapon_Socket")));
}

void UPlayerAttackANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

}