#include "Skill/SkillR.h"

void ASkillR::UseSkill(AActor* Target)
{
	Super::UseSkill(Target);

	UE_LOG(LogTemp, Warning, TEXT("Use SkillR!"));

	bool bCanUseSkill = GetWorldTimerManager().IsTimerActive(SkillTimer);
	if (bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("Remain CoolTime is %f!"), GetWorldTimerManager().GetTimerRemaining(SkillTimer));
		return;
	}

	UAnimInstance* Anim = OwnActor->GetMesh()->GetAnimInstance();
	if (!Anim || !BladeMontage)
	{
		return;
	}
	// 플레이어가 검기를 날리는 애니메이션 실행.
	Anim->Montage_Play(BladeMontage);
	// 더미를 스폰하는 횟수를 0으로 초기화시킨다.
	SpawnNumber = 0;
	// 타이머를 이용해서 n초동안 x초마다 검기를 날리는 잔상을 스폰.
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASkillR::SpawnDummy, SpawnTime, true);
}

void ASkillR::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillR Restored!"));
}

void ASkillR::SpawnDummy()
{
	// 잔상 더미를 스폰한다.
	// 잔상 스폰은 BeginPlay()에서 바로 검기를 날리는 몽타주를 실행한다.
	// 이 때 노티파이 클래스를 이용하여 애니메이션 실행시 검기를 스폰하여 날리고
	// 애니메이션이 끝나면 Destroy()를 이용하여 사라진다.
	
	// 스폰 횟수를 늘린다.
	++SpawnNumber;
	// 만약 스폰 횟수가 주어진 수보다 같거나 커졌다면 타이머를 해체하고 그대로 탈출한다.
	if (SpawnNumber <= 4)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		return;
	}
}
