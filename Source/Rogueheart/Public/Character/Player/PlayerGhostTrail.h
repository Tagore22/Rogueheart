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
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "CapSuleCollider")
	class UCapsuleComponent* Collider;

	UPROPERTY(EditAnywhere, Category = "Material")
	class UMaterialInterface* Material = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* BladeMontage = nullptr;
};
