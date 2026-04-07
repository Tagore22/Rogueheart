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
        // TryGetPawnOwner()가 nullptr을 반환하더라도 Cast<>()에서 그대로 nullptr을
        // 반환하기 때문에 가독성을 높일 수 있었다. 모든 Getter함수가 유효성 체크를
        // 하지 않고 모든 Try함수도 유효성 체크를 하지 않기에 일일히 찾아보거나
        // 아니면 맘편하게 IsValid()를 사용하여야 한다.
        Player = Cast<APlayerCharacter>(TryGetPawnOwner());
    }

    if (!IsValid(Player))
        return;

    // Velocity는 단순히 수치 계산을 위해 쓰인다. 이동, 회전, 방향에 쓰일 경우에는
    // FVector::IsNearlyZero()를 통해 걸러내야할 수도 있다.
    FVector Velocity = Player->GetVelocity();

    // z축의 이동값까지 끼게 되면 값이 튀어버린다. 따라서 z값은 버린다.
    FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.f);

    // 루트 모션을 이용한 애니메이션을 사용시 속도의 조절은 불필요함.
    // 애니메이션이 주도권을 쥐고 있다. ABP에서 트랜지션의 조건값이기 
    // 때문에 없앨 수 없다.
    Speed = HorizontalVelocity.Size();

    // 락온 상태 확인. ABP에서 트랜지션의 조건값이기에 없앨 수 없음.
    bIsLockedOn = IsValid(Player->LockOnTarget);

    if (bIsLockedOn)
    {
        // 아래 내적보다 FQuat 연산을 통해 한번에 알아낼 수 있다.
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

        // 블렌드 스페이스의 두 변수들.
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

    Player = Cast<APlayerCharacter>(TryGetPawnOwner());
}

// 노티파이 연동시키는거 잊어버렸다. 여기부터 볼 것.

// 몽타주에 노티파이를 만들어놓고 이곳에서 AnimNotify_뒤에 그 노티파이명으로
// 함수를 만들어놓으면 델리게이트에 의해 연동되어 알아서 호출됨.
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