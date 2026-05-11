#include "WeaponBase.h"
#include "Item/ItemData.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::SetupWeapon(const FItemData& ItemData)
{
	UStaticMesh* Mesh = ItemData.PickupMesh.LoadSynchronous();
	WeaponMesh->SetStaticMesh(Mesh);
}

