#include "Skill/SkillQ.h"

void ASkillQ::UseSkill(AActor* Target)
{
	Super::UseSkill(Target);

	bool bCanUseSkill = GetWorldTimerManager().IsTimerActive(SkillTimer);
	if (bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("Remain CoolTime is %f!"), GetWorldTimerManager().GetTimerRemaining(SkillTimer));
		return;
	}

	GetWorldTimerManager().SetTimer(SkillTimer, this, &ASkillQ::RestoreSkill, Data.Cooldown, false);

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
	}
}

void ASkillQ::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillQ Restored!"));
}

