#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Soul.generated.h"

UCLASS()
class ROGUEHEART_API ASoul : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoul();

	//virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSoulOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UCapsuleComponent* CapsuleCom = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	class UNiagaraComponent* EffectCom = nullptr;

	UPROPERTY(EditAnywhere, Category = "SoulNum")
	int32 SoulNum = 100;
};
