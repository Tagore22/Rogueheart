#include "WeaponSweepComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Rogueheart.h"

/*UWeaponSweepComponent::UWeaponSweepComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}*/

void UWeaponSweepComponent::BeginPlay()
{
	Super::BeginPlay();

	const ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!IsValid(Owner))
		return;
	const USkeletalMeshComponent* MeshComp = Owner->GetMesh();
	if (!MeshComp)
		return;

	// TArray를 통해서 존재하는 모든 소켓들의 이전 위치들을 모두 구해놓는다. 다만 이것은 이후에도 적용되는 위치가 아닌
	// TArray를 구축하기 위한 위치들이므로 이후 반드시 갱신이 필요하다.
	for (int i = 0; i < 5; ++i)
	{
		FString SocketName = FString::Printf(TEXT("Weapon_Socket%d"), i);
		PrevSocketLocations.Add(MeshComp->GetSocketLocation(FName(*SocketName)));
	}

	// 각 액터의 SweepComponent가 에디터에서 어떻게 설정되어있는지에 따라 트레이스가 나뉘어진다.
	// 반드시 설정해야만 한다.
	switch (TraceType)
	{
	case ETraceChannel::Player:
		Channel = TraceChannel::ECC_Player;
		break;

	case ETraceChannel::Enemy:
		Channel = TraceChannel::ECC_Enemy;
		break;
	}
}

// Tick()이 필요한가 마지막까지 확인할 것. 필요없다면 생성자에 false로 바꿔라.
/*void UWeaponSweepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}*/

void UWeaponSweepComponent::ClearHitActors()
{
	HitActors.Empty();
}

FVector UWeaponSweepComponent::GetPrevSocketLocation(int32 AttackIndex) const
{
	if (AttackIndex >= 5)
	{
		return FVector::ZeroVector;
	}
	return PrevSocketLocations[AttackIndex];
}

void UWeaponSweepComponent::SetPrevSocketLocation(const FVector& Location, int32 AttackIndex)
{
	PrevSocketLocations[AttackIndex] = Location;
}

FVector UWeaponSweepComponent::GetCurSocketLocation() const
{
	return CurSocketLocation;
}

void UWeaponSweepComponent::SetCurSocketLocation(const FVector& Location)
{
	CurSocketLocation = Location;
}

// 콤보인 양손의 소켓 번호는 0, 4
void UWeaponSweepComponent::SweepAttack(const FVector& Location, int32 AttackIndex, bool bIsKnockback)
{
	//PrevSocketLocations[AttackIndex] = CurSocketLocation;
	CurSocketLocation = Location;

	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(OutHits, PrevSocketLocations[AttackIndex], CurSocketLocation, FQuat::Identity, Channel, FCollisionShape::MakeSphere(SweepLength), Params);

	// 궤도 확인 전용 더버깅.
	//DrawDebugLine(GetWorld(), PrevSocketLocations[AttackIndex], CurSocketLocation, FColor::Yellow, false, 1.0f, 0, 2.0f);
	if (!bHit)
		return;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!IsValid(Owner))
		return;

	for (FHitResult Hit : OutHits)
	{
		ACharacter* HitActor = Cast<ACharacter>(Hit.GetActor());
		if (IsValid(HitActor) && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			UGameplayStatics::ApplyDamage(HitActor, SweepDamage, Owner->GetController(), Owner, nullptr);
			if (bIsKnockback)
			{
				FVector Dir = HitActor->GetActorLocation() - Owner->GetActorLocation();
				HitActor->LaunchCharacter(Dir * LaunchForce, true, false);
			}
		}
	}
	// 마지막에 현재 소켓 좌표를 이전 소켓 좌표로 갱신후에 함수 종료.
	PrevSocketLocations[AttackIndex] = CurSocketLocation;
}