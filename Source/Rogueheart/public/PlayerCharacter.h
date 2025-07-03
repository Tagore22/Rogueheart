#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class YOURGAME_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

protected:
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
    UPROPERTY(EditAnywhere, Category="Camera")
    float BaseTurnRate;

    UPROPERTY(EditAnywhere, Category="Camera")
    float BaseLookUpRate;
};
