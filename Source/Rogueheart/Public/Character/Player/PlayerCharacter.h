#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Skill/SkillComponent.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

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

    // 상태 설정 함수
    void SetPlayerState(EPlayerState NewState);
    bool IsDodging() const { return CurrentState == EPlayerState::Dodging; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 입력 처리 함수들
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    void Dodge(const FInputActionValue& Value);

    // 스킬 사용 함수
    UFUNCTION()
        void UseFireball();

    UFUNCTION()
        void UseIceBlast();

    // 상태 체크 함수
    bool CanAct() const;

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

    // 캐릭터 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
        EPlayerState CurrentState;

    // 입력 설정
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

    // 공격 / 회피 애니메이션
    UPROPERTY(EditDefaultsOnly, Category = "Attack")
        UAnimMontage* AMT_Attack;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        float DodgeDistance = 600.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        float DodgeSpeed = 2400.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        UAnimMontage* AMT_Dodge;

    // 스킬 시스템
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
        USkillComponent* SkillComponent;

    // UI
    UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> WB_SkillCooldownClass;

    UUserWidget* CooldownWidget;
};