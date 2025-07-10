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

    // 스킬 배열 초기화 후 타이머 0으로 세팅
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

    // 스킬 데이터 찾기
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

    // 애니메이션 몽타주
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

    // 파티클 효과
    if (Data.Effect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            Data.Effect,
            Owner->GetActorLocation(),
            Owner->GetActorRotation()
        );
    }

    // 사운드
    if (Data.Sound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            Data.Sound,
            Owner->GetActorLocation()
        );
    }

    // TODO: 데미지나 힐 등의 실제 효과 로직 추가
}