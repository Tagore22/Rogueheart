#include "Skill/SkillComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"  // FMath

USkillComponent::USkillComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USkillComponent::BeginPlay()
{
    Super::BeginPlay();

    // ��ų �迭 �ʱ�ȭ �� Ÿ�̸� 0���� ����
    for (const FSkillData& Skill : Skills)
    {
        CooldownTimers.Add(Skill.SkillID, 0.f);
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
        }
    }
}

bool USkillComponent::UseSkill(FName SkillID)
{
    float* Remaining = CooldownTimers.Find(SkillID);
    if (!Remaining || *Remaining > 0.f)
    {
        return false;
    }

    // ��ų ������ ã��
    const FSkillData* DataPtr = Skills.FindByPredicate([&](const FSkillData& D) {
        return D.SkillID == SkillID;
        });
    if (!DataPtr)
    {
        return false;
    }

    ExecuteSkill(*DataPtr);
    CooldownTimers[SkillID] = DataPtr->Cooldown;
    return true;
}

void USkillComponent::ExecuteSkill(const FSkillData& Data)
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    // �ִϸ��̼� ��Ÿ��
    if (Data.Montage)
    {
        if (USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>())
        {
            if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
            {
                AnimInst->Montage_Play(Data.Montage);
            }
        }
    }

    // ��ƼŬ ȿ��
    if (Data.Effect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            Data.Effect,
            Owner->GetActorLocation(),
            Owner->GetActorRotation()
        );
    }

    // ����
    if (Data.Sound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            Data.Sound,
            Owner->GetActorLocation()
        );
    }

    // TODO: �������� �� ���� ���� ȿ�� ���� �߰�
}