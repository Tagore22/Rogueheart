#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemData.h"
#include "ItemPickup.generated.h"

UCLASS()
class ROGUEHEART_API AItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemPickup();

	// 이거 하나만 둠 (에디터에서 전체 ItemData를 직접 입력)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* TriggerSphere;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPickup(AActor* Picker);

public:	
	//virtual void Tick(float DeltaTime) override;
};
