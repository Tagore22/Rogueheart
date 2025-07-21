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
    Collision->SetCollisionProfileName(TEXT("SkillOverlap"));
    Collision->SetGenerateOverlapEvents(true);
    RootComponent = Collision;

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ASkillActor::OnSkillOverlap);

    SkillEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillEffect"));
    SkillEffect->SetupAttachment(RootComponent);
    SkillEffect->SetAutoDestroy(true);
    SkillEffect->bAutoActivate = false;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = Collision;

    SkillDamage = 0.f;
}

void ASkillActor::BeginPlay()
{
    Super::BeginPlay();

    // �⺻ ����Ʈ ���
    if (SkillEffect && CurrentSkillData.Effect)
    {
        SkillEffect->SetAsset(CurrentSkillData.Effect);
        SkillEffect->Activate(true);
    }

    // ���� ����
    if (CurrentSkillData.LifeTime > 0.f)
    {
        SetLifeSpan(CurrentSkillData.LifeTime);
    }

    // ��ų�� ����
    ActivateSkill();
}

void ASkillActor::InitializeSkill(const FSkillDataRow& SkillData, AActor* SkillOwner, float Damage)
{
    CurrentSkillData = SkillData;
    Caster = SkillOwner;

    // �浹 �ݰ� ����
    if (Collision)
    {
        Collision->SetSphereRadius(SkillData.Radius);
    }

    // ���޹��� ������ ����
    SkillDamage = Damage;
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
        Explode();
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

    // ���� SkillDamage ���
    UGameplayStatics::ApplyDamage(OtherActor, SkillDamage, nullptr, this, CurrentSkillData.DamageType);
    UE_LOG(LogTemp, Warning, TEXT("Applied Damage : %f"), SkillDamage);

    Explode();
}

void ASkillActor::Explode()
{
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

// Fireball: ProjectileMovement Ȱ��ȭ �� ���� ����ü
void ASkillActor::Activate_Fireball()
{
    if (ProjectileMovement)
    {
        FVector Velocity = GetActorForwardVector() * CurrentSkillData.Speed;
        ProjectileMovement->Velocity = Velocity;
        ProjectileMovement->ProjectileGravityScale = 0.f;
        ProjectileMovement->Activate(true);
    }
}

// Ice Nova: �ݰ� �� �� ��� ������
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
            UGameplayStatics::ApplyDamage(Other, SkillDamage, nullptr, this, CurrentSkillData.DamageType);
        }
    }

    Explode();
}

// Thunder Strike: ���� ���� Trace �� ��� ������
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
            UGameplayStatics::ApplyDamage(HitActor, SkillDamage, nullptr, this, CurrentSkillData.DamageType);
        }
        DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Blue, false, 1.0f, 0, 2.0f);
    }

    Explode();
}

// Dash Slash: ĳ���� �����̵� �� �浹 �� ������
void ASkillActor::Activate_DashSlash()
{
    if (Caster)
    {
        FVector DashTo = Caster->GetActorLocation() + Caster->GetActorForwardVector() * CurrentSkillData.Range;
        Caster->SetActorLocation(DashTo, true);

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
                UGameplayStatics::ApplyDamage(Other, SkillDamage, nullptr, this, CurrentSkillData.DamageType);
        }
    }

    Explode();
}

// Healing Light: �ݰ� �� �Ʊ� ȸ�� (Damage < 0 �̸� Heal)
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
            float HealAmount = -SkillDamage;
            // IHealthInterface* HI = Cast<IHealthInterface>(Other);
            // if (HI) HI->Execute_Heal(Other, HealAmount);
        }
    }

    Explode();
}