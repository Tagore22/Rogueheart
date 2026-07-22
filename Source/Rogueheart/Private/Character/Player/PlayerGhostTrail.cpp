#include "Character/Player/PlayerGhostTrail.h"
#include "Components/CapsuleComponent.h"

APlayerGhostTrail::APlayerGhostTrail()
{
	//PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapSuleCollider"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(Collider);
}

void APlayerGhostTrail::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
	{
		Mesh->SetMaterial(i, Material);
	}

	UAnimInstance* Anim = Mesh->GetAnimInstance();
	if (!Anim || !BladeMontage)
	{
		return;
	}
	// 플레이어가 검기를 날리는 애니메이션 실행.
	Anim->Montage_Play(BladeMontage);
}

/*void APlayerGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

