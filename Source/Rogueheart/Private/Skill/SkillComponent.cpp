#include "Skill/SkillComponent.h"
#include "Skill/SkillDataRow.h"
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

    if (SkillDataTable)
    {
        TArray<FName> RowNames = SkillDataTable->GetRowNames();
        for (const FName& Name : RowNames)
        {
            CooldownTimers.Add(Name, 0.f);
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

void USkillComponent::UseSkill(const FName& SkillRowName)
{
    if (!SkillDataTable)
        return;

    float& Timer = CooldownTimers.FindOrAdd(SkillRowName);
    if (Timer > 0.f)
        return;

    FSkillDataRow* SkillData = SkillDataTable->FindRow<FSkillDataRow>(SkillRowName, TEXT("UseSkill"));
    if (!SkillData)
        return;

    UClass* LoadedClass = SkillData->SkillClass.LoadSynchronous();
    if (!LoadedClass)
        return;

    UWorld* World = GetWorld();
    if (!World)
        return;

    FVector Location = GetOwner()->GetActorLocation();
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
        Skill->InitializeSkill(*SkillData, GetOwner());
        UGameplayStatics::FinishSpawningActor(Skill, FTransform(Rotation, Location));
    }

    Timer = SkillData->Cooldown;
}