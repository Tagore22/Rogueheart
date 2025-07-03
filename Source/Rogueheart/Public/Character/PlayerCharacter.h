#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle,
    Moving,
    Attacking,
    Dodging,
    Parrying,
    Stunned
};

UCLASS()
class ROGUEHEART_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 이동 입력 함수
    void MoveForward(float Value);
    void MoveRight(float Value);

    // 카메라 입력 함수
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    // 점프
    virtual void Jump() override;
    virtual void StopJumping() override;

private:
    // 카메라 회전 속도
    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseTurnRate;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseLookUpRate;

    // 카메라 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    // 플레이어 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState;
};