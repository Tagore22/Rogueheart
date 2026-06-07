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
	UWeaponSweepComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ClearHitActors();

	FVector GetPrevSocketLocation() const;
	void SetPrevSocketLocation(const FVector& Location);

	FVector GetCurSocketLocation() const;
	void SetCurSocketLocation(const FVector& Location);

	void SweepAttack(const FVector& Location);

private:
	TArray<AActor*> HitActors;

	FVector PrevSocketLocation = FVector(0.f, 0.f, 0.f);

	FVector CurSocketLocation = FVector(0.f, 0.f, 0.f);;

	UPROPERTY(EditDefaultsOnly, Category = "SweepLength")
	float SweepLength = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "SweepDamage")
	float SweepDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "TraceChannel")
	ETraceChannel TraceType;

	ECollisionChannel Channel;
};
