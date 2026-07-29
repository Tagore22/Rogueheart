#include "Soul.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

ASoul::ASoul()
{
	//PrimaryActorTick.bCanEverTick = true;

	CapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleCom;

	EffectCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	EffectCom->SetupAttachment(RootComponent);
}

/*void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

void ASoul::BeginPlay()
{
	Super::BeginPlay();

	CapsuleCom->OnComponentBeginOverlap.AddDynamic(this, &ASoul::OnSoulOverlap);
	EffectCom->Activate();
}

void ASoul::OnSoulOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player") || !EffectCom)
	{
		return;
	}
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (!IsValid(Player))
	{
		return;
	}
	Player->SetSoulSum(SoulNum);

	EffectCom->Deactivate();

	Destroy();
}
