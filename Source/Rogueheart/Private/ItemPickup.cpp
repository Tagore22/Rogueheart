#include "ItemPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AItemPickup::AItemPickup()
{
	// 지금 당장은 쓸 일이 없다. 프로젝트 종료 혹은 사용할 일이 생길시 변경.
	// PrimaryActorTick.bCanEverTick = true;

    // 루트 컴포넌트
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // 메시 컴포넌트
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 물리 충돌 끄기

    // 트리거 구체
    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    TriggerSphere->SetupAttachment(RootComponent);
    TriggerSphere->SetSphereRadius(100.f);  // 기본 겹침 범위 100cm
}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	
    // 에디터에서 ItemData가 이미 설정되어 있으니 바로 사용
    // 지금 당장은 별게 없다. 에디터에서 각 BP마다 DT값을 직접 초기화하고 있기 
    // 때문이다. 이후 하나씩 옮겨오도록 하자.
    if (!ItemData.PickupMesh.IsNull())
    {
        UStaticMesh* LoadedMesh = ItemData.PickupMesh.LoadSynchronous();
        if (LoadedMesh)
        {
            MeshComponent->SetStaticMesh(LoadedMesh);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AItemPickup: PickupMesh load fail - %s"), *ItemData.ItemName.ToString());
        }
    }
    TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnTriggerOverlap);
    // 추가 초기화 (예: 회전 애니메이션 등은 여기서 시작 가능)
}

void AItemPickup::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어인지 체크 (태그로 간단히, 나중에 클래스 체크로 강화 가능)
    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
    {
        OnPickup(OtherActor);
    }
}

void AItemPickup::OnPickup(AActor* Picker)
{
    // 지금은 로그 + 파괴 (테스트용)
    if (!ItemData.ItemName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Item Get : %s"), *ItemData.ItemName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Item Get : (No Item Name)"));
    }
    Destroy();
}

/*void AItemPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

