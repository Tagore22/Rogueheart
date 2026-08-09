#include "WeaponSweepComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Rogueheart.h"

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

	for (int i = 0; i < 5; ++i)
	{
		FString SocketName = FString::Printf(TEXT("Weapon_Socket%d"), i);
		PrevSocketLocations.Add(MeshComp->GetSocketLocation(FName(*SocketName)));
	}

	switch (TraceType)
	{
	case ETraceChannel::Player:
		Channel = TraceChannel::ECC_Player;
		break;

	case ETraceChannel::Enemy:
		Channel = TraceChannel::ECC_Enemy;
		break;
	}
	if (Channel == TraceChannel::ECC_Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Player"), *Owner->GetName());
	}
	else if (Channel == TraceChannel::ECC_Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Enemy"), *Owner->GetName());
	}
	else
	{

	}
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

FVector UWeaponSweepComponent::GetPrevSocketLocation(int32 AttackIndex) const
{
	if (AttackIndex <= 5)
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

void UWeaponSweepComponent::SweepAttack(const FVector& Location, int32 AttackIndex)
{
	CurSocketLocation = Location;

	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(OutHits, PrevSocketLocations[AttackIndex], CurSocketLocation, FQuat::Identity, Channel, FCollisionShape::MakeSphere(SweepLength), Params);
	if (!bHit)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Attack Start"));
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
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
			UGameplayStatics::ApplyDamage(HitActor, SweepDamage, Owner->GetController(), Owner, nullptr);
		}
	}
	// 마지막에 현재 소켓 좌표를 이전 소켓 좌표로 갱신후에 함수 종료.
	PrevSocketLocations[AttackIndex] = CurSocketLocation;
}