#include "Character/Player/MoveComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UMoveComponent::SetupInputBinding(UEnhancedInputComponent* EnhancedInput)
{
	Super::SetupInputBinding(EnhancedInput);

    EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UMoveComponent::Move);
    EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UMoveComponent::Look);
}

void UMoveComponent::Move(const FInputActionValue& Value)
{
    if (!Player->CanAct(EActionType::Move))
        return;

    // 지역 변수를 새로 만들 땐 const를 이용하여 의도의 명확성을 추가함.
    // value.Get<>()에서 캐스팅되는 타입은 에디터에서 해당 InputAction에 설정한
    // 값에 따라 다르다. Axis1D = float, Axis2D = FVector2D, Axis3D = FVector이다.
    const FVector2D MovementVector2D = Value.Get<FVector2D>();
    if (MovementVector2D.IsNearlyZero())
        return;

    LastMoveInput = FVector(MovementVector2D.X, MovementVector2D.Y, 0.f);

    const FRotator Rotation = Player->GetControlRotation();
    const FRotationMatrix YawMatrix = FRotator(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = YawMatrix.GetUnitAxis(EAxis::X);
    const FVector RightDirection = YawMatrix.GetUnitAxis(EAxis::Y);

    Player->AddMovementInput(ForwardDirection, MovementVector2D.X);
    Player->AddMovementInput(RightDirection, MovementVector2D.Y);
}

void UMoveComponent::Look(const FInputActionValue& Value)
{
    if (IsValid(LockOnTarget)) // LockOnTarget을 PlayerBaseComponent로 옮김.
        return;

    const FVector2D LookAxis = Value.Get<FVector2D>();
    if (LookAxis.IsNearlyZero())
        return;

    Player->AddControllerYawInput(LookAxis.X);
    Player->AddControllerPitchInput(LookAxis.Y);
}