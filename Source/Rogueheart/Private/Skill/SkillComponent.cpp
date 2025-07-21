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

    for (const FName& RowName : SkillDataTable->GetRowNames())
    {
        const FSkillDataRow* Row = SkillDataTable->FindRow<FSkillDataRow>(RowName, TEXT("BeginPlay"));
        if (Row)
        {
            SkillRowNames.Add(Row->SkillType, RowName);
            CooldownTimers.Add(Row->SkillType, 0.f);

            // 레벨 초기화 (레벨 0부터 시작)
            SkillLevels.Add(RowName, 0);
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
    const FSkillDataRow* SkillData = GetSkillData(SkillType);
    if (!SkillData)
        return;

    float& Timer = CooldownTimers.FindOrAdd(SkillType);
    if (Timer > 0.f)
        return;
    UpgradeSkill(SkillType);
    // 여기서 데미지 계산
    float Damage = GetSkillDamage(SkillType);

    // 대미지 넘겨주면서 액터 스폰
    SpawnSkillActor(*SkillData, Damage);

    // 쿨다운 시작
    Timer = SkillData->Cooldown;
}

void USkillComponent::SpawnSkillActor(const FSkillDataRow& SkillData, float Damage)
{
    UClass* LoadedClass = SkillData.SkillClass.LoadSynchronous();
    if (!LoadedClass)
        return;

    UWorld* World = GetWorld();
    if (!World || !GetOwner())
        return;

    FVector Location = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 150.f;
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
        Skill->InitializeSkill(SkillData, GetOwner(), Damage);
        UGameplayStatics::FinishSpawningActor(Skill, FTransform(Rotation, Location));
    }
}

// 스킬 업그레이드 함수
bool USkillComponent::UpgradeSkill(ESkillType SkillType)
{
    const FSkillDataRow* SkillData = GetSkillData(SkillType);
    if (!SkillData) return false;

    const FName* RowName = SkillRowNames.Find(SkillType);
    if (!RowName) return false;

    int32& CurrentLevel = SkillLevels.FindOrAdd(*RowName);
    int32 MaxLevel = SkillData->DamagePerLevel.Num() - 1;

    if (CurrentLevel >= MaxLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("Skill %s is already at max level."), *RowName->ToString());
        return false;
    }

    CurrentLevel++;
    UE_LOG(LogTemp, Log, TEXT("Skill %s upgraded to Level %d"), *RowName->ToString(), CurrentLevel);
    return true;
}

// 현재 데미지 반환 함수
float USkillComponent::GetSkillDamage(ESkillType SkillType) const
{
    const FSkillDataRow* SkillData = GetSkillData(SkillType);
    if (!SkillData) return 0.f;

    const FName* RowName = SkillRowNames.Find(SkillType);
    if (!RowName) return 0.f;

    const int32* LevelPtr = SkillLevels.Find(*RowName);
    int32 Level = (LevelPtr) ? *LevelPtr : 0;
    UE_LOG(LogTemp, Log, TEXT("SkillLevel 값: %d"), Level);

    if (SkillData->DamagePerLevel.IsValidIndex(Level))
        return SkillData->DamagePerLevel[Level];

    return SkillData->DamagePerLevel.Last();
}