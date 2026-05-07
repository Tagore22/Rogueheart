#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::SetupWeapon(TSoftObjectPtr<UStaticMesh> PickupMesh)
{
	UStaticMesh* Mesh = PickupMesh.LoadSynchronous();
	WeaponMesh->SetStaticMesh(Mesh);
}

