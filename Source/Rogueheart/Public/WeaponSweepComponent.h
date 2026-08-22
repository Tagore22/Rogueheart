#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSweepComponent.generated.h"

UENUM(BlueprintType)
enum class ETraceChannel : uint8
{
	Player,
	Enemy
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUEHEART_API UWeaponSweepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//UWeaponSweepComponent();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ClearHitActors();

	FVector GetPrevSocketLocation(int32 AttackIndex) const;
	void SetPrevSocketLocation(const FVector& Location, int32 AttackIndex);

	FVector GetCurSocketLocation() const;
	void SetCurSocketLocation(const FVector& Location);

	void SweepAttack(const FVector& Location, int32 AttackIndex, bool bIsKnockback = false);

private:
	TArray<AActor*> HitActors;

	TArray<FVector> PrevSocketLocations;

	FVector CurSocketLocation;

	UPROPERTY(EditDefaultsOnly, Category = "SweepLength")
	float SweepLength = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "SweepDamage")
	float SweepDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchForce")
	float LaunchForce = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "TraceChannel")
	ETraceChannel TraceType;

	ECollisionChannel Channel;
};
