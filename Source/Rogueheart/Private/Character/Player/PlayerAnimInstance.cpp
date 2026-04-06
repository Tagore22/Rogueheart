#include "Character/Player/PlayerAnimInstance.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!IsValid(Player))
    {
        APawn* OwningPawn = TryGetPawnOwner();
        if (IsValid(OwningPawn))
        {
            Player = Cast<APlayerCharacter>(OwningPawn);
        }
    }

    if (!IsValid(Player))
        return;

    FVector Velocity = Player->GetVelocity();
    FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.f);

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
        
        // 아래 연산을 어려워할 것이 전혀 없다. 조금 정리해보자면
        // HorizontalVelocity는 월드 벡터이다. 단순히 월드 좌표로써
        // 어느 방향만큼 얼마나 움직였는 지를 뜻한다. 이상태에서 정면, 우측
        // 속도인 WalkSpeed, Direct를 알아내기 위해서는 HorizontalVelocity를
        // 현재 액터의 역회전만큼 회전시켜 액터의 로컬 벡터로 만드는 것이다.
        // 조금더 자세히 예를 들면 액터가 동쪽을 바라본채로 북쪽으로 움직였다.
        // 여기서 북쪽으로의 움직임은 액터와는 전혀 관계없는 월드 벡터다.
        // 이것을 액터에 써먹어야하는데 동쪽을 바라봤을 때의 북쪽으로의 움직임은
        // 즉 왼쪽으로 움직임인데 이것은 동쪽으로의 회전을 서쪽으로 역회전시키므로써
        // 북쪽으로의 움직임이 왼쪽으로의 움직임을 파악할 수 있다.
        FVector RelativeVelocity = Player->GetActorQuat().UnrotateVector(HorizontalVelocity);
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

    APawn* OwningPawn = TryGetPawnOwner();

    if (OwningPawn)
    {
        Player = Cast<APlayerCharacter>(OwningPawn);
    }
}

// 노티파이 연동시키는거 잊어버렸다. 여기부터 볼 것.
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
    if (IsValid(Player))
    {
        Player->SetPlayerState(EPlayerState::Idle);
        Player->RestoreLockOnIfNeeded();
    }
}