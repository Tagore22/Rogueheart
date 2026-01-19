#include "Character/Player/PlayerAnimInstance.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
    Speed = 0.f;
    //bIsInAir = false;
    //bIsAccelerating = false;
    //bIsAttacking = false;

    WalkSpeed = 0.f;
    Direct = 0.f;
    bIsLockedOn = false;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwningPawn = TryGetPawnOwner();
    if (!OwningPawn) 
        return;

    // 혹여라도 BeginPlay()에서 Player를 캐싱하지 못했을 때 여기서 마지막으로
    // 찾아본다.
    if (!IsValid(Player))
    {
        Player = Cast<APlayerCharacter>(OwningPawn);
    }

    FVector Velocity = OwningPawn->GetVelocity();
    FVector HorizontalVelocity(Velocity.X, Velocity.Y, 0.f);

    // 루트 모션을 이용한 애니메이션을 사용시 속도의 조절은 불필요함.
    // 애니메이션이 주도권을 쥐고 있다. ABP에서 트랜지션의 조건값이기 
    // 때문에 없앨 수 없다.
    Speed = HorizontalVelocity.Size();

    /*if (const UCharacterMovementComponent* Movement = Player->GetCharacterMovement())
    {
        bIsInAir = Movement->IsFalling();
        // 0과의 비교라면 굳이 제곱근을 사용하는 Size()보다 SizeSquared()가 낫다.
        bIsAccelerating = Movement->GetCurrentAcceleration().SizeSquared() > 0.f;
    }*/

    // 락온 상태 확인. ABP에서 트랜지션의 조건값이기에 없앨 수 없음.
    bIsLockedOn = IsValid(Player->LockOnTarget);

    if (bIsLockedOn)
    {
        /*FVector Velocity = OwningPawn->GetVelocity();
        FVector Forward = OwningPawn->GetActorForwardVector();
        FVector Right = OwningPawn->GetActorRightVector();

        FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
        WalkSpeed = FVector::DotProduct(HorizontalVelocity, Forward);
        Direct = FVector::DotProduct(HorizontalVelocity, Right);*/
        //
        FVector RelativeVelocity = OwningPawn->GetActorQuat().UnrotateVector(HorizontalVelocity);
        // 블론드 스페이스의 두 변수들.
        WalkSpeed = RelativeVelocity.X;
        Direct = RelativeVelocity.Y;
    }
    else
    {
        WalkSpeed = 0.f;
        Direct = 0.f;
    }
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* Owner = TryGetPawnOwner();

    if (Owner)
    {
        Player = Cast<APlayerCharacter>(Owner);
    }
}

void UPlayerAnimInstance::AnimNotify_EndAttack()
{
    if (IsValid(Player))
    {
        Player->OnAttackEnd();
    }
    //SetIsAttacking(false);
}

void UPlayerAnimInstance::AnimNotify_NextCombo()
{
    if (IsValid(Player))
    {
        Player->bCanNextCombo = true;
    }
}

void UPlayerAnimInstance::AnimNotify_EndDodge()
{
    ResetPlayerToIdle();
}

void UPlayerAnimInstance::ResetPlayerToIdle()
{
    if (IsValid(Player))
    {
        Player->SetPlayerState(EPlayerState::Idle);
        Player->RestoreLockOnIfNeeded();
    }
}