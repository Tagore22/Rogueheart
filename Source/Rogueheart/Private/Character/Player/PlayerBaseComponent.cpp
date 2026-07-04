#include "Character/Player/PlayerBaseComponent.h"

UPlayerBaseComponent::UPlayerBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerBaseComponent::SetLockOnState(bool bIsLockOn)
{
    if (bIsLockOn)
    {
        Player->GetCharacterMovement()->bOrientRotationToMovement = false;
        Player->bUseControllerRotationYaw = true;
    }
    else
    {
        Player->GetCharacterMovement()->bOrientRotationToMovement = true;
        Player->bUseControllerRotationYaw = false;
    }
}

void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerBaseComponent::SetupInputBinding(UEnhancedInputComponent* EnhancedInput)
{

}

// Dodge 애니메이션의 재생이 끝나면 호출되는 노티파이인
// UPlayerAnimInstance::AnimNotify_EndDodge()에서 이 함수를 호출한다.
// 락온 상태에서 구르기 행동시에만 발동하여 PrevLockOnTarget의 처리를 담당한다.
void UPlayerBaseComponent::RestoreLockOnIfNeeded() //
{
    if (!IsValid(PrevLockOnTarget))
        return;

    LockOnTarget = PrevLockOnTarget;
    SetLockOnState(true);
    PrevLockOnTarget = nullptr;
}

bool UPlayerBaseComponent::HasLockTarget() const
{
    return LockOnTarget == nullptr ? false : true;
}

