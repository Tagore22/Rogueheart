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
    UPROPERTY(EditAnywhere, Category="Camera")
    float BaseTurnRate;

    UPROPERTY(EditAnywhere, Category="Camera")
    float BaseLookUpRate;
};
