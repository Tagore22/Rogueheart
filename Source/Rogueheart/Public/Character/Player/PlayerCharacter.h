#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaChanged);

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle,
    Moving,
    Attacking,
    LockedOn,
    Dodging,
    Damaged,
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

UCLASS()
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
    void OnAttackEnd();
    void RestoreLockOnIfNeeded();
    void HealPlayer(float PlusHP);
    void SetEquippedWeapon(class AWeaponBase* CurWeapon);
    void SetWeaponVisible(bool IsVisible);
    bool HasLockTarget() const;
    void SetCanNextComboTrue();
    void ClearLockOn();
    void OnActStart();

protected:
    virtual void PossessedBy(AController* NewController) override;

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
    void SetLockOnTarget(class AEnemyBase* NewTarget);
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    class AEnemyBase* FindNearestTarget();
    void UpdateLockOnRotation(float DeltaTime);
    class AEnemyBase* SwitchTarget(bool bLeft);
    void CheckLockOnDistance();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxHP() const;
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurHP() const;
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxStamina() const;
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurStamina() const;

protected:
    // 잘 정리해서 public이 아니라 private쪽으로 넘길 것. meta = AllowPrivateAccess는 BP에서 접근하는 게 아니면 쓸모가 없다.
    // 에디터에서 접근하는 건 없어도 된다.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Look;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Attack;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Dodge;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Skill1;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Skill2;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_LockOn;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_SwitchTargetLeft;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_SwitchTargetRight;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_InventoryOnOff;
 
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<class UAnimMontage*> AttackMontages;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<class UAnimMontage*> DamagedMontages;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    class UAnimMontage* AMT_Dodge;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USkillComponent* SkillComponent;

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

    UPROPERTY(VisibleAnywhere, Category = "Targeting")
    class AEnemyBase* LockOnTarget = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Targeting")
    class AEnemyBase* PrevLockOnTarget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LockOnRange = 1200.f;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LockOnBreakDistance = 1500.f;

    UPROPERTY(VisibleAnywhere, Category = "SweepComponent")
    class UWeaponSweepComponent* SweepCom = nullptr;

    float LockOnBreakDistanceSq = 0.f;

    bool bWasLockedOnWhenDodged = false;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState = EPlayerState::Idle;

    FVector LastMoveInput = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    uint8 TeamID = 1;

    void SetLockOnState(bool bIsLockOn);

    void ConsumeStamina(const float Cost);

    bool CanPlusStamina() const;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float TargetingAngle = 0.5f;
    // StatSubsystem으로 옮겨야 하는 변수들.
    UPROPERTY(EditDefaultsOnly, Category = "HP")
    float MaxHP = 100.f;
    UPROPERTY(EditDefaultsOnly, Category = "HP")
    float CurHP = 50.f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float MaxStamina = 100.f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float CurStamina = 100.f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float StaminaCost = 10.f;
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float PlusStamina = 2.f;

    UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = "true"))
    FOnHPChanged OnHPChanged;

    UPROPERTY()
    class ARogueheartPlayerController* CachedController = nullptr;
    UPROPERTY()
    class AWeaponBase* EquippedWeapon = nullptr;

    const ECollisionChannel TraceChannel = ECC_GameTraceChannel1;
};