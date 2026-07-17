#include "Skill/SkillQ.h"
#include "AIController.h"

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

	ACharacter* Enemy = Cast<ACharacter>(Target);
	if (!IsValid(Enemy))
	{
		return;
	}
	//OwnActor

	// Enemy의 후측 벡터를 찾아낸다.
	AAIController* AICon = Enemy->GetController<AAIController>();
	FRotator AIRotator = FRotator(0.f, AICon->GetControlRotation().Yaw, 0.f);
	FVector BackVec = AIRotator.Vector() * - 1;
	// 그 벡터의 정면으로 50만큼 이동한 위치를 알아낸다.
	FVector MovePosition = Enemy->GetActorLocation() + BackVec * 50.f;
	// OwnActor의 방향을 Enemy의 방향으로 돌려 마치 그림자 밟기처럼 구현한다.
	// 하지만 이미 타겟팅된 적쪽을 계속 바라보기 때문에 필요없을 수도 있다.
	//FRotator EnemyRotator = Enemy->GetActorRotation();
	//OwnActor->SetActorRotation(EnemyRotator);
	// OwnActor를 그 위치로 이동시키되 SetActorLocation()을 쓰고 두번째 매개변수를 쓴다.
	bool bIsMoved = OwnActor->SetActorLocation(MovePosition, true);
	if (bIsMoved)
	{
		UE_LOG(LogTemp, Warning, TEXT("Move Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Move fail"));
	}
}

void ASkillQ::RestoreSkill()
{
	Super::RestoreSkill();

	UE_LOG(LogTemp, Warning, TEXT("SkillQ Restored!"));
}

