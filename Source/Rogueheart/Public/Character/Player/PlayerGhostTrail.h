#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerGhostTrail.generated.h"

UCLASS()
class ROGUEHEART_API APlayerGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerGhostTrail();

	//virtual void Tick(float DeltaTime) override;

	TSubclassOf<class AActor> GetBlade() const { return Blade; }

	float GetPlusDistance() const { return PlusDistance; }

	float GetBladeDamage() const { return BladeDamage; }

	void InitializePGT(float Damage) { BladeDamage = Damage; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "PlusDistance")
	float PlusDistance = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "CapSuleCollider")
	class UCapsuleComponent* Collider;

	UPROPERTY(EditAnywhere, Category = "Material")
	class UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* BladeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "BladeGhostTrail")
	TSubclassOf<class AActor> Blade;

	float BladeDamage;
};
