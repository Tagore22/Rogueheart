#include "Skill/SkillComponent.h"
#include "Skill/SkillActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

USkillComponent::USkillComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USkillComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!SkillDataTable)
        return;

    // DataTable�� ��� �ο츦 ��ȸ�ϸ� SkillType �� RowName, �ʱ� ��ٿ� ����
    for (const FName& RowName : SkillDataTable->GetRowNames())
    {
        const FSkillDataRow* Row = SkillDataTable->FindRow<FSkillDataRow>(RowName, TEXT("BeginPlay"));
        if (Row)
        {
            SkillRowNames.Add(Row->SkillType, RowName);
            CooldownTimers.Add(Row->SkillType, 0.f);
        }
    }
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateCooldowns(DeltaTime);
}

void USkillComponent::UpdateCooldowns(float DeltaTime)
{
    for (auto& Elem : CooldownTimers)
    {
        if (Elem.Value > 0.f)
        {
            Elem.Value = FMath::Max(0.f, Elem.Value - DeltaTime);
            OnSkillCooldownUpdated.Broadcast(Elem.Key, Elem.Value);
        }
    }
}

const FSkillDataRow* USkillComponent::GetSkillData(ESkillType SkillType) const
{
    if (const FName* RowName = SkillRowNames.Find(SkillType))
    {
        return SkillDataTable->FindRow<FSkillDataRow>(*RowName, TEXT("GetSkillData"));
    }
    return nullptr;
}

void USkillComponent::UseSkill(ESkillType SkillType)
{
    // DataTable ���� �� ��ٿ� üũ
    const FSkillDataRow* SkillData = GetSkillData(SkillType);
    if (!SkillData)
        return;

    float& Timer = CooldownTimers.FindOrAdd(SkillType);
    if (Timer > 0.f)
        return;

    // ��ų ���� ����
    SpawnSkillActor(*SkillData);

    // ��ٿ� ����
    Timer = SkillData->Cooldown;
}

void USkillComponent::SpawnSkillActor(const FSkillDataRow& SkillData)
{
    UClass* LoadedClass = SkillData.SkillClass.LoadSynchronous();
    if (!LoadedClass)
        return;

    UWorld* World = GetWorld();
    if (!World || !GetOwner())
        return;

    FVector Location = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 150.f; // �÷��̾��� ��ġ���� ���� �Ÿ� �տ��� ����
    FRotator Rotation = GetOwner()->GetActorRotation();

    ASkillActor* Skill = World->SpawnActorDeferred<ASkillActor>(
        LoadedClass,
        FTransform(Rotation, Location),
        GetOwner(),
        nullptr,
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

    if (Skill)
    {
        Skill->InitializeSkill(SkillData, GetOwner());
        UGameplayStatics::FinishSpawningActor(Skill, FTransform(Rotation, Location));
    }
}