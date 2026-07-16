#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalBase.generated.h"

UCLASS()
class ROGUEHEART_API APortalBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapPortal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	//virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UCapsuleComponent* CapsuleCom;

	UPROPERTY(VisibleAnywhere, Category = "StaticMesh")
	class UStaticMeshComponent* MeshCom;
};
