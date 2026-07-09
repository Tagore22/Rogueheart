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
    Parrying,
    Dead
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
    Move,
    Attack,
    LockOn,
    Dodge,
    Parry,
    UseSkill
};

UCLASS()
class ROGUEHEART_API APlayerCharacter : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    APlayerCharacter();

    virtual class UCameraComponent* GetCameraComponent() const { return FollowCamera; }
    virtual FGenericTeamId GetGenericTeamId() const override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 플레이어의 상태에 관한 것은 그대로 남겨둔다.
    void SetPlayerState(EPlayerState NewState);
    bool CanAct(EActionType DesiredAction) const;
    // bool IsDodging() const { return CurrentState == EPlayerState::Dodging; }
    bool IsAttacking() const { return CurrentState == EPlayerState::Attacking; }
    class ARogueheartPlayerController* GetCachedController() const { return CachedController; }

    //void PlayComboMontage(); // a
    void OnAttackEnd(); // a. 래핑함수.
    void RestoreLockOnIfNeeded(); // t. 래핑함수.
    void HealPlayer(float PlusHP); // s
    void SetEquippedWeapon(class AWeaponBase* CurWeapon);
    void SetWeaponVisible(bool IsVisible);
    bool HasLockTarget() const; // p. 래핑함수.
    void SetCanNextComboTrue(); // a. 래핑함수.
    //void ClearLockOn(); // t
    void OnActStart();
    void UseSkill(const struct FInputActionInstance& Instance);
    // void SetLockOnTarget(class AEnemyBase* NewTarget);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxHP() const; // s
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurHP() const; // s
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxStamina() const; // s
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurStamina() const; // s

protected:
    virtual void PossessedBy(AController* NewController) override;

    //void Move(const struct FInputActionValue& Value); // m
    //void Look(const struct FInputActionValue& Value); // m
    //void Attack(const struct FInputActionValue& Value); // a
    //void Dodge(const struct FInputActionValue& Value); // m
    UFUNCTION()
    void UseFireball(const struct FInputActionValue& Value);
    UFUNCTION()
    void UseIceBlast(const struct FInputActionValue& Value);
    //void ToggleLockOn(const struct FInputActionValue& Value); // t
    //void SwitchTargetLeft(const struct FInputActionValue& Value); // t
    //void SwitchTargetRight(const struct FInputActionValue& Value); // t
    void ToggleInventory(const struct FInputActionValue& Value);
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    //class AEnemyBase* FindNearestTarget(); // t
    //void UpdateLockOnRotation(float DeltaTime); // t
    //class AEnemyBase* SwitchTarget(bool bLeft); // t
    //void CheckLockOnDistance(); // t

protected:
    // 잘 정리해서 public이 아니라 private쪽으로 넘길 것. meta = AllowPrivateAccess는 BP에서 접근하는 게 아니면 쓸모가 없다.
    // 에디터에서 접근하는 건 없어도 된다.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    //UPROPERTY(EditDefaultsOnly, Category = "Input")
    //class UInputAction* IA_Move; // m

    //UPROPERTY(EditDefaultsOnly, Category = "Input")
    //class UInputAction* IA_Look; // m

    //UPROPERTY(EditDefaultsOnly, Category = "Input")
    //class UInputAction* IA_Attack; // a

    //UPROPERTY(EditDefaultsOnly, Category = "Input")
    //class UInputAction* IA_Dodge; // m

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Skill1;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Skill2;

    //UPROPERTY(EditDefaultsOnly, Category = "Input")
    //class UInputAction* IA_LockOn; // t

    //UPROPERTY(EditDefaultsOnly, Category = "Input")
    //class UInputAction* IA_SwitchTargetLeft; // t

    //UPROPERTY(EditDefaultsOnly, Category = "Input")
    //class UInputAction* IA_SwitchTargetRight; // t

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_InventoryOnOff;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Skill_Q;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Skill_E;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Skill_R;
 
    //UPROPERTY(EditDefaultsOnly, Category = "Animation")
    //TArray<class UAnimMontage*> AttackMontages; // a

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<class UAnimMontage*> DamagedMontages;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<class UAnimMontage*> DieMontages;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    //class UAnimMontage* AMT_Dodge; // m

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USkillComponent* SkillComponent;

    //UPROPERTY(BlueprintReadWrite, Category = "Combat")
    //int32 CurrentCombo = 0; // a

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    //int32 MaxCombo = 3; // a

    //UPROPERTY(EditDefaultsOnly, Category = "Combat")
    //int32 InterpSpeed = 5; // t

    //UPROPERTY(BlueprintReadWrite, Category = "Combat")
    //bool bInputCombo = false; // a

    //UPROPERTY(BlueprintReadWrite, Category = "Combat")
    //bool bCanNextCombo = false; // a

    //UPROPERTY(VisibleAnywhere, Category = "Targeting")  
    //class AEnemyBase* LockOnTarget = nullptr; // t

    //UPROPERTY(VisibleAnywhere, Category = "Targeting")
    //class AEnemyBase* PrevLockOnTarget = nullptr; // t

    //UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    //float LockOnRange = 1200.f; // t

    //UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    //float LockOnBreakDistance = 1500.f; // t

    UPROPERTY(VisibleAnywhere, Category = "SweepComponent")
    class UWeaponSweepComponent* SweepCom = nullptr;

    //float LockOnBreakDistanceSq = 0.f; // t

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState = EPlayerState::Idle;

    //FVector LastMoveInput = FVector::ZeroVector; // m

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    uint8 TeamID = 1;

    //void SetLockOnState(bool bIsLockOn); // p

    void ConsumeStamina(const float Cost);

    bool CanPlusStamina() const;

    //UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    //float TargetingAngle = 0.5f; // t
    // StatSubsystem으로 옮겨야 하는 변수들.
    UPROPERTY(EditDefaultsOnly, Category = "HP")
    float MaxHP = 100.f; // s
    UPROPERTY(EditDefaultsOnly, Category = "HP")
    float CurHP = 50.f; // s
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float MaxStamina = 100.f; // s
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float CurStamina = 100.f; // s
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float StaminaCost = 10.f; // s
    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float PlusStamina = 2.f; // s

    UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = "true"))
    FOnHPChanged OnHPChanged;

    UPROPERTY()
    class ARogueheartPlayerController* CachedController = nullptr; // p

    UPROPERTY()
    class AWeaponBase* EquippedWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "SkillComponent")
    class USkillBaseComponent* SkillBaseCom = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "MoveComponent")
    class UMoveComponent* MoveCom = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "AttackComponent")
    class UAttackComponent* AttackCom = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "TargetComponent")
    class UTargetComponent* TargetCom = nullptr;
};