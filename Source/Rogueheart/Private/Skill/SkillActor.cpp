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

    // 데이터 기반 이펙트 자동 플레이
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

    // 충돌 반경 설정
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

    // 데미지 적용
    UGameplayStatics::ApplyDamage(OtherActor, CurrentSkillData.Damage, nullptr, this, nullptr);
    Explode();
}

void ASkillActor::Explode()
{
    // 임팩트 이펙트
    if (CurrentSkillData.Effect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            CurrentSkillData.Effect,
            GetActorLocation(),
            GetActorRotation()
        );
    }

    // 액터 제거
    Destroy();
}
