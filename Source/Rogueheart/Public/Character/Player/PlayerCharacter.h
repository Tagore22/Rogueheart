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
class UAnimMontage;
class AEnemyBase;

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

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetPlayerState(EPlayerState NewState);
    bool CanAct() const;
    bool IsDodging() const { return CurrentState == EPlayerState::Dodging; }
    bool IsAttacking() const { return CurrentState == EPlayerState::Attacking; }

    void PlayComboMontage();
    void HandleComboInput();
    void OnAttackEnd();

    void RestoreLockOnIfNeeded();

protected:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    void Dodge(const FInputActionValue& Value);

    UFUNCTION()
    void UseFireball();

    UFUNCTION()
    void UseIceBlast();

    void ToggleLockOn();
    void FindNearestTarget();
    void UpdateLockOnRotation(float DeltaTime);

public:
    // 카메라
    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseTurnRate = 45.f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float BaseLookUpRate = 45.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;


    // 입력
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

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_LockOn;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_SwitchTargetLeft;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_SwitchTargetRight;


    // 애니메이션
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* AMT_Attack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* AMT_Dodge;


    // 스킬 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkillComponent* SkillComponent;


    // 전투 관련 변수
    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    int32 CurrentCombo = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    int32 MaxCombo = 3;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bInputCombo = false;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bCanNextCombo = false;


    // UI
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> WB_SkillCooldownClass;

    UPROPERTY()
    UUserWidget* CooldownWidget = nullptr;


    // 타겟팅
    UPROPERTY(BlueprintReadOnly, Category = "Targeting")
    AActor* LockOnTarget = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Targeting")
    bool bIsLockedOn = false;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LockOnRange = 1200.f;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LockOnBreakDistance = 1500.f;

    bool bWasLockedOnWhenDodged = false;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState = EPlayerState::Idle;

    FVector LastMoveInput = FVector::ZeroVector;

    void SwitchTarget(bool bLeft);
    void SwitchTargetLeft();
    void SwitchTargetRight();
    void CheckLockOnDistance();

    // 이전 타겟 기억용 (UI 마커 제어에 필요)
    AEnemyBase* CurrentTargetEnemy = nullptr;
};