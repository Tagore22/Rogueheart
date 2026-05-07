#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class ROGUEHEART_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetupWeapon(TSoftObjectPtr<UStaticMesh> PickupMesh);

private:
	UPROPERTY()
	UStaticMeshComponent* WeaponMesh;
};
