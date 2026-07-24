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
	ProjectCom->SetUpdatedComponent(Collider);
	ProjectCom->InitialSpeed = 500;
	ProjectCom->MaxSpeed = 500;
	ProjectCom->ProjectileGravityScale = 0.f;

	BladeCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	BladeCom->SetupAttachment(RootComponent);
}

void ABladeGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ABladeGhostTrail::OnBladeOverlap);

	//ProjectCom->Velocity = GetActorForwardVector() * 500.f;

	//ProjectCom->Activate();

	FTimerHandle BladeTimer;
	GetWorldTimerManager().SetTimer(BladeTimer, this, &ABladeGhostTrail::DestroyActor, DestroyTimer, false);
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
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	Destroy();
}

void ABladeGhostTrail::DestroyActor() 
{
	Destroy();
}

