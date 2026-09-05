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

	UE_LOG(LogTemp, Warning, TEXT("Portal Overlap!"));

	StatSub->SetMaxHP(Player->GetMaxHP());
	StatSub->SetCurHP(Player->GetCurHP());

	StatSub->SetMaxMP(Player->GetMaxMana());
	StatSub->SetCurMP(Player->GetCurMana());

	StatSub->SetMaxStamina(Player->GetMaxStamina());
	StatSub->SetCurStamina(Player->GetCurStamina());

	StatSub->SetSoulSum(Player->GetSoulSum());

	StatSub->SetHPLevel(Player->GetHPLevel());

	StatSub->SetMPLevel(Player->GetMPLevel());

	StatSub->SetStaminaLevel(Player->GetStaminaLevel());

	UE_LOG(LogTemp, Warning, TEXT("MaxHP : %f"), Player->GetMaxHP());
	UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), Player->GetCurHP());
	UE_LOG(LogTemp, Warning, TEXT("MaxMP : %f"), Player->GetMaxMana());
	UE_LOG(LogTemp, Warning, TEXT("CurMP : %f"), Player->GetCurMana());
	UE_LOG(LogTemp, Warning, TEXT("MaxStamina : %f"), Player->GetMaxStamina());
	UE_LOG(LogTemp, Warning, TEXT("CurStamina : %f"), Player->GetCurStamina());
	UE_LOG(LogTemp, Warning, TEXT("SoulSum : %d"), Player->GetSoulSum());

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("PortalTestMap"));
}

/*void APortalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

