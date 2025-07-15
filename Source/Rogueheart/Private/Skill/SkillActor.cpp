#include "Skill/SkillActor.h"
#include "Skill/SkillDataRow.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASkillActor::ASkillActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(50.f);
    Collision->SetCollisionProfileName(TEXT("OverlapAll"));
    RootComponent = Collision;
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ASkillActor::OnSkillOverlap);

    SkillEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillEffect"));
    SkillEffect->SetupAttachment(RootComponent);
    SkillEffect->bAutoActivate = false;
}

void ASkillActor::BeginPlay()
{
    Super::BeginPlay();

    // ������ ��� ����Ʈ �ڵ� �÷���
    if (SkillEffect && CurrentSkillData.Effect)
    {
        SkillEffect->SetAsset(CurrentSkillData.Effect);
        SkillEffect->Activate(true);
    }

    if (CurrentSkillData.LifeTime > 0.f)
        SetLifeSpan(CurrentSkillData.LifeTime);
}

void ASkillActor::InitializeSkill(const FSkillDataRow& SkillData, AActor* SkillOwner)
{
    CurrentSkillData = SkillData;
    Caster = SkillOwner;

    // �浹 �ݰ� ����
    if (Collision)
        Collision->SetSphereRadius(SkillData.Radius);
}

void ASkillActor::OnSkillOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == Caster)
        return;

    // ������ ����
    UGameplayStatics::ApplyDamage(OtherActor, CurrentSkillData.Damage, nullptr, this, nullptr);
    Explode();
}

void ASkillActor::Explode()
{
    // ����Ʈ ����Ʈ
    if (CurrentSkillData.Effect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            CurrentSkillData.Effect,
            GetActorLocation(),
            GetActorRotation()
        );
    }

    // ���� ����
    Destroy();
}
