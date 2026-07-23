#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BladeGhostTrail.generated.h"

UCLASS()
class ROGUEHEART_API ABladeGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	ABladeGhostTrail();

	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
    void OnBladeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	//UPROPERTY(EditAnywhere, Category = "Effect")
	//class UNiagaraSystem* BladeEffect;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	class UNiagaraComponent* BladeCom;

	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UCapsuleComponent* Collider;

	UPROPERTY(VisibleAnywhere, Category = "ProjectileMovement")
	class UProjectileMovementComponent* ProjectCom;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float BladeDamage = 20.f;
};
