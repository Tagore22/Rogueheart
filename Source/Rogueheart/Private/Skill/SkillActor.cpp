#include "Skill/SkillActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ASkillActor::ASkillActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(50.f);
    Collision->SetCollisionProfileName(TEXT("SkillOverlap")); // 필요시 Project Settings > Collision 설정
    Collision->SetGenerateOverlapEvents(true);
    RootComponent = Collision;

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ASkillActor::OnSkillOverlap);

    SkillEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillEffect"));
    SkillEffect->SetupAttachment(RootComponent);
    SkillEffect->SetAutoDestroy(true);
    SkillEffect->bAutoActivate = false;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = Collision;
}

void ASkillActor::BeginPlay()
{
    Super::BeginPlay();

    // 기본 이펙트 재생
    if (SkillEffect && CurrentSkillData.Effect)
    {
        SkillEffect->SetAsset(CurrentSkillData.Effect);
        SkillEffect->Activate(true);
    }

    // 수명 설정
    if (CurrentSkillData.LifeTime > 0.f)
    {
        SetLifeSpan(CurrentSkillData.LifeTime);
    }

    // 스킬별 실행
    ActivateSkill();
}

void ASkillActor::InitializeSkill(const FSkillDataRow& SkillData, AActor* SkillOwner)
{
    CurrentSkillData = SkillData;
    Caster = SkillOwner;

    // 충돌 반경 세팅
    if (Collision)
    {
        Collision->SetSphereRadius(SkillData.Radius);
    }
}

void ASkillActor::ActivateSkill()
{
    const FName& Name = CurrentSkillData.SkillName;

    if (Name == "Fireball")
        Activate_Fireball();
    else if (Name == "Ice Nova")
        Activate_IceNova();
    else if (Name == "Thunder Strike")
        Activate_ThunderStrike();
    else if (Name == "Dash Slash")
        Activate_DashSlash();
    else if (Name == "Healing Light")
        Activate_HealingLight();
    else
        Explode();  // 기본 동작
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

    // 데미지 적용 예시 (필요 시 수정 가능)
    UGameplayStatics::ApplyDamage(OtherActor, CurrentSkillData.Damage, nullptr, this, CurrentSkillData.DamageType);
    UE_LOG(LogTemp, Warning, TEXT("FireAttack"));

    Explode();
}

void ASkillActor::Explode()
{
    // 임팩트 이펙트 재생
    if (CurrentSkillData.Effect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            CurrentSkillData.Effect,
            GetActorLocation(),
            GetActorRotation()
        );
    }
    Destroy();
}

// ========================== 스킬별 구현 ==========================

// 1) Fireball: ProjectileMovement 활성화 → 직선 투사체
void ASkillActor::Activate_Fireball()
{
    if (ProjectileMovement)
    {
        FVector Velocity = GetActorForwardVector() * CurrentSkillData.Speed;
        ProjectileMovement->Velocity = Velocity;
        ProjectileMovement->ProjectileGravityScale = 0.f; // 이후 지울수도
        ProjectileMovement->Activate(true);
    }
}

// 2) Ice Nova: BeginPlay 직후 반경 내 적 모두 데미지
void ASkillActor::Activate_IceNova()
{
    TArray<FOverlapResult> overlaps;
    FCollisionQueryParams params(NAME_None, false, Caster);
    GetWorld()->OverlapMultiByChannel(
        overlaps,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(CurrentSkillData.Radius),
        params
    );

    for (auto& R : overlaps)
    {
        if (AActor* Other = R.GetActor())
        {
            UGameplayStatics::ApplyDamage(Other, CurrentSkillData.Damage, nullptr, this, CurrentSkillData.DamageType);
        }
    }

    Explode();
}

// 3) Thunder Strike: 전방 직선 Trace 후 맞은 대상만 데미지
void ASkillActor::Activate_ThunderStrike()
{
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * CurrentSkillData.Range;

    FHitResult Hit;
    FCollisionQueryParams params(NAME_None, false, Caster);
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, params))
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            UGameplayStatics::ApplyDamage(HitActor, CurrentSkillData.Damage, nullptr, this, CurrentSkillData.DamageType);
        }

        // 디버그용 선 그리기
        DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Blue, false, 1.0f, 0, 2.0f);
    }

    Explode();
}

// 4) Dash Slash: 캐스터를 앞으로 순간이동시키며 충돌시 데미지
void ASkillActor::Activate_DashSlash()
{
    if (Caster)
    {
        FVector DashTo = Caster->GetActorLocation() + Caster->GetActorForwardVector() * CurrentSkillData.Range;
        Caster->SetActorLocation(DashTo, true);

        // 이동 후 근거리 적에게 데미지
        TArray<FOverlapResult> overlaps;
        GetWorld()->OverlapMultiByChannel(
            overlaps,
            DashTo,
            FQuat::Identity,
            ECC_Pawn,
            FCollisionShape::MakeSphere(CurrentSkillData.Radius),
            FCollisionQueryParams(NAME_None, false, Caster)
        );

        for (auto& R : overlaps)
        {
            if (AActor* Other = R.GetActor())
                UGameplayStatics::ApplyDamage(Other, CurrentSkillData.Damage, nullptr, this, CurrentSkillData.DamageType);
        }
    }

    Explode();
}

// 5) Healing Light: 반경 내 아군 회복
void ASkillActor::Activate_HealingLight()
{
    TArray<FOverlapResult> overlaps;
    GetWorld()->OverlapMultiByChannel(
        overlaps,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(CurrentSkillData.Radius),
        FCollisionQueryParams(NAME_None, false, Caster)
    );

    for (auto& R : overlaps)
    {
        if (AActor* Other = R.GetActor())
        {
            float HealAmount = -CurrentSkillData.Damage;  // Damage < 0 → Heal
            // 예: 캐스팅 타입체크 후 Heal 인터페이스 호출 후에 인터페이스 제작후 추가할 것
            //IHealthInterface* HI = Cast<IHealthInterface>(Other);
            //if (HI) HI->Execute_Heal(Other, HealAmount);
        }
    }

    Explode();
}