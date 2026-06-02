#include "WeaponSweepComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Rogueheart.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Enemy/EnemyBase.h"

UWeaponSweepComponent::UWeaponSweepComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponSweepComponent::BeginPlay()
{
	Super::BeginPlay();

	const ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!IsValid(Owner))
		return;
	const USkeletalMeshComponent* MeshComp = Owner->GetMesh();
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

	bool bHit = GetWorld()->SweepMultiByChannel(OutHits, PrevSocketLocation, CurSocketLocation, FQuat::Identity, TraceChannel::Enemy, FCollisionShape::MakeSphere(SweepLength), Params);
	if (!bHit)
		return;

	APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwner());
	if (!IsValid(Owner))
		return;

	for (FHitResult Hit : OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (IsValid(HitActor) && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			UE_LOG(LogTemp, Warning, TEXT("Another Actor Attacked!"));
			// 여기서 HitActor의 ApplyDamage()를 호출한다. 20은 임시이며, 이후 추가 수정할 것.
			UGameplayStatics::ApplyDamage(HitActor, 20.f, Owner->GetController(), Owner, nullptr);
			/*AEnemyBase* HitEnemy = Cast<AEnemyBase>(HitActor);
			if (!IsValid(HitEnemy))
				return;
			HitEnemy->ShowHPBarWidget(true);
			if (HitEnemy->GetCurHP() <= 0.f)
			{
				Owner->ClearLockOn();
			}*/
		}
	}
	// 마지막에 현재 소켓 좌표를 이전 소켓 좌표로 갱신후에 함수 종료.
	PrevSocketLocation = CurSocketLocation;
}
