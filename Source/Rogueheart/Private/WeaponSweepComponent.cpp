#include "WeaponSweepComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Rogueheart.h"

UWeaponSweepComponent::UWeaponSweepComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponSweepComponent::BeginPlay()
{
	Super::BeginPlay();

	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (!IsValid(Player))
		return;
	const USkeletalMeshComponent* MeshComp = Player->GetMesh();
	if (!MeshComp)
		return;
	PrevSocketLocation = MeshComp->GetSocketLocation(TEXT("Weapon_Socket"));
}

// Tick()이 필요한가 마지막까지 확인할 것. 필요없다면 생성자에 false로 바꿔라.
void UWeaponSweepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponSweepComponent::ClearHitActors()
{
	HitActors.Empty();
}

FVector UWeaponSweepComponent::GetPrevSocketLocation() const
{
	return PrevSocketLocation;
}

void UWeaponSweepComponent::SetPrevSocketLocation(const FVector& Location)
{
	PrevSocketLocation = Location;
}

FVector UWeaponSweepComponent::GetCurSocketLocation() const
{
	return CurSocketLocation;
}

void UWeaponSweepComponent::SetCurSocketLocation(const FVector& Location)
{
	CurSocketLocation = Location;
}

void UWeaponSweepComponent::SweepAttack(const FVector& Location)
{
	CurSocketLocation = Location;

	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(OutHits, PrevSocketLocation, CurSocketLocation, FQuat::Identity, TraceChannel::Enemy, FCollisionShape::MakeSphere(20.f), Params);
	if (!bHit)
		return;

	for (FHitResult Hit : OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (IsValid(HitActor) && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			// 여기서 HitActor의 ApplyDamage()를 호출한다.
		}
	}
	// 마지막에 현재 소켓 좌표를 이전 소켓 좌표로 갱신후에 함수 종료.
	PrevSocketLocation = CurSocketLocation;
}
