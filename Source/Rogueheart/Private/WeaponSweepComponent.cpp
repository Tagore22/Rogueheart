#include "WeaponSweepComponent.h"

UWeaponSweepComponent::UWeaponSweepComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponSweepComponent::BeginPlay()
{
	Super::BeginPlay();
}

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
