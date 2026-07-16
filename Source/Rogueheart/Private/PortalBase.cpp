#include "PortalBase.h"
#include "Components/CapsuleComponent.h"
#include "StatSubsystem.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

APortalBase::APortalBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = CapsuleCom;

	MeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshCom->SetupAttachment(RootComponent);
}

void APortalBase::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleCom->OnComponentBeginOverlap.AddDynamic(this, &APortalBase::OnOverlapPortal);
}

void APortalBase::OnOverlapPortal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UStatSubsystem* StatSub = GetGameInstance()->GetSubsystem<UStatSubsystem>();

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (!StatSub || !IsValid(Player))
	{
		return;
	}

	StatSub->SetMaxHP(Player->GetMaxHP());
	StatSub->SetCurHP(Player->GetCurHP());
	StatSub->SetMaxStamina(Player->GetMaxStamina());
	StatSub->SetCurStamina(Player->GetCurStamina());

	UE_LOG(LogTemp, Warning, TEXT("Portal Overlap!"));

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("PortalTestMap"));
}

/*void APortalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

