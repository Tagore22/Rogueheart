#include "Skill/SkillQ.h"

void ASkillQ::UseSkill(AActor* Target, int32 SkillLevel)
{
	Super::UseSkill(Target, SkillLevel);

	bool bCanUseSkill = GetWorldTimerManager().IsTimerActive(SkillTimer);
	float Cost = OwnActor->GetCurMana();
	// SkillBase가 가지고 있는 OwnActor의 타입명은 
	// if OwnActor의 태그가 플레이어라면 아래 Cost를 받는다.
	/*if (OwnActor->ActorHasTag(SkillNames::PlayerTag) && OwnActor->GetCurMana() <= 0.f)
	{
		return;
	}*/
	// if Cost <= 0.f라면 return; 아래 if문의 Cost는 삭제한다.
	if (bCanUseSkill || Cost <= 0.f || !Data.Material)
	{
		UE_LOG(LogTemp, Warning, TEXT("Remain CoolTime is %f!"), GetWorldTimerManager().GetTimerRemaining(SkillTimer));
		return;
	}

	GetWorldTimerManager().SetTimer(SkillTimer, this, &ASkillQ::RestoreSkill, Data.Cooldown[SkillLevel], false);

	USkeletalMeshComponent* Mesh = OwnActor->GetMesh();

	for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
	{
		Materials.Add(Mesh->GetMaterial(i));
	}

	UE_LOG(LogTemp, Warning, TEXT("Use SkillQ!"));

	// Enemy의 후측 벡터를 찾아낸다.
	FVector BackVec = Target->GetActorForwardVector() * -1;
	// 그 벡터의 정면으로 50만큼 이동한 위치를 알아낸다.
	FVector MovePosition = Target->GetActorLocation() + BackVec * 50.f;
	// 만약 적이 벽을 등지고 있다던가로 이동하려는 위치의 공간이 없을 수도 있기에 검사해본다.
	bool bCanTeleport = GetWorld()->FindTeleportSpot(OwnActor, MovePosition, OwnActor->GetActorRotation());
	// 반환값이 true라면 상황은 다음과 같은 2가지이다.
	// 1. 기존의 MovePosition에 순간이동이 가능함.
	// 2. 기존의 MovePosition에 순간이동은 불가능하지만 주위에 가능한 공간이 있으며 알아서 MovePosition의 값을 수정해줌.
	// FindTeleportSpot의 위치를 입력받는 2번째 매개변수의 타입명이 벡터의 참조자이기 때문.
	// 따라서 반환값이 true라면 MovePosition으로 이동하면 된다.
	if (bCanTeleport)
	{
		OwnActor->SetActorLocation(MovePosition);
		// if 태그가 플레이어라면.
		/*if (OwnActor->ActorHasTag(SkillNames::PlayerTag))
		{
			OwnActor->CostMana(Data.Cost[SkillLevel]);
		}*/

		OwnActor->CostMana(Data.Cost[SkillLevel]);

		UMaterialInterface* Mat = Data.Material.LoadSynchronous();
		for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
		{
			Mesh->SetMaterial(i, Mat);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("SkillQ Level : %d"), SkillLevel);
}

void ASkillQ::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillQ Restored!"));

	USkeletalMeshComponent* Mesh = OwnActor->GetMesh();

	for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
	{
		Mesh->SetMaterial(i, Materials[i]);
	}
}

