#include "Skill/BladeGhostTrail.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABladeGhostTrail::ABladeGhostTrail()
{
	//PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = Collider;

	ProjectCom = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectCom->SetUpdatedComponent(RootComponent);
	ProjectCom->InitialSpeed = 1500;
	ProjectCom->MaxSpeed = 1500;
	ProjectCom->Velocity = GetActorForwardVector() * 1500.f;

	BladeCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	BladeCom->SetupAttachment(RootComponent);
}

void ABladeGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ABladeGhostTrail::OnBladeOverlap);
}

/*void ABladeGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

void ABladeGhostTrail::OnBladeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AController* PC = GetWorld()->GetFirstPlayerController();
	UGameplayStatics::ApplyDamage(OtherActor, BladeDamage, PC, PC->GetOwner(), nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Blade Attack!"));
	Destroy();
}

