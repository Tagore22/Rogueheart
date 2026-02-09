#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaChanged);

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UUserWidget;
class UAnimMontage;
class USkillComponent;
class AEnemyBase;
struct InputActionValue;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle,
    Moving,
    Attacking,
    LockedOn,
    Dodging,
    Stunned,
    Dead
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
    Move,
    Attack,
    LockOn,
    Dodge,
    UseSkill
};

UCLASS(Blueprintable, BlueprintType)
class ROGUEHEART_API APlayerCharacter : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    APlayerCharacter();

    virtual FGenericTeamId GetGenericTeamId() const override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetPlayerState(EPlayerState NewState);
    bool CanAct(EActionType DesiredAction) const;
    bool IsDodging() const { return CurrentState == EPlayerState::Dodging; }
    bool IsAttacking() const { return CurrentState == EPlayerState::Attacking; }

    void PlayComboMontage();
    void HandleComboInput();
    void OnAttackEnd();
    void RestoreLockOnIfNeeded();
    void HealPlayer(float PlusHP);

protected:
    void Move(const struct FInputActionValue& Value);
    void Look(const struct FInputActionValue& Value);
    void Attack(const struct FInputActionValue& Value);
    void Dodge(const struct FInputActionValue& Value);
    UFUNCTION()
    void UseFireball(const struct FInputActionValue& Value);
    UFUNCTION()
    void UseIceBlast(const struct FInputActionValue& Value);
    void ToggleLockOn(const struct FInputActionValue& Value);
    void SwitchTargetLeft(const struct FInputActionValue& Value);
    void SwitchTargetRight(const struct FInputActionValue& Value);
    void ToggleInventory(const struct FInputActionValue& Value);

    AEnemyBase* FindNearestTarget();
    void UpdateLockOnRotation(float DeltaTime);
    AEnemyBase* SwitchTarget(bool bLeft);
    void CheckLockOnDistance();
    void ClearLockOn();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxHP() const;
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurHP() const;
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxStamina() const;
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurStamina() const;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

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

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_InventoryOnOff;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* AMT_Attack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* AMT_Dodge;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkillComponent* SkillComponent;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    int32 CurrentCombo = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    int32 MaxCombo = 3;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    int32 InterpSpeed = 5;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bInputCombo = false;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bCanNextCombo = false;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> WBP_SkillCooldownClass;

    UPROPERTY()
    UUserWidget* CooldownWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> WBP_Inventory;

    UPROPERTY()
    UUserWidget* InventoryWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> WBP_HPBar;

    UPROPERTY()
    UUserWidget* HPBarWidget = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Targeting")
    AEnemyBase* LockOnTarget = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Targeting")
    AEnemyBase* PrevLockOnTarget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LockOnRange = 1200.f;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float MinAngle = 120.f;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LockOnBreakDistance = 1500.f;

    float LockOnBreakDistanceSq;

    bool bWasLockedOnWhenDodged = false;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState = EPlayerState::Idle;

    FVector LastMoveInput = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    uint8 TeamID = 1;

    void SetLockOnState(bool bIsLockOn);

    float MaxHP = 100.f;
    float CurHP = 50.f;
    float MaxStamina = 100.f;
    float CurStamina = 100.f;

    UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = "true"))
    FOnHPChanged OnHPChanged;
};