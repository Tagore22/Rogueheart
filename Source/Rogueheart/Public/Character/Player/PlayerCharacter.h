#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Skill/SkillComponent.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UUserWidget;

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

    // 상태 제어
    void SetPlayerState(EPlayerState NewState);
    bool CanAct() const;
    bool IsDodging() const { return CurrentState == EPlayerState::Dodging; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 입력 처리
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    void Dodge(const FInputActionValue& Value);

    // 스킬 처리
    UFUNCTION()
        void UseFireball();

    UFUNCTION()
        void UseIceBlast();

private:
    // ===== 상태 =====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState = EPlayerState::Idle;

    // ===== 카메라 =====
    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseTurnRate = 45.f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseLookUpRate = 45.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // ===== 입력 매핑 =====
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Look;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Attack;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Dodge;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Skill1;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Skill2;

    // ===== 애니메이션 =====
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* AMT_Attack;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* AMT_Dodge;

    // ===== 스킬 시스템 =====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkillComponent* SkillComponent;

    // ===== UI =====
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> WB_SkillCooldownClass;

    UUserWidget* CooldownWidget = nullptr;
};