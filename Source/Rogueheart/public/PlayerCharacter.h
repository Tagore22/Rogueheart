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

    // �̵� �Է� �Լ�
    void MoveForward(float Value);
    void MoveRight(float Value);

    // ī�޶� �Է� �Լ�
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    // ����
    virtual void Jump() override;
    virtual void StopJumping() override;

private:
    // ī�޶� ȸ�� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseTurnRate;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseLookUpRate;

    // ī�޶� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    // �÷��̾� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState;
};