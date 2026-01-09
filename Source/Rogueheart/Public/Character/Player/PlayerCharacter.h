#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Skill/SkillComponent.h"
#include "GenericTeamAgentInterface.h"
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
    AEnemyBase* FindNearestTarget();
    void UpdateLockOnRotation(float DeltaTime);
    AEnemyBase* SwitchTarget(bool bLeft);
    void SwitchTargetLeft();
    void SwitchTargetRight();
    void CheckLockOnDistance();
    void ClearLockOn();

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

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bInputCombo = false;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bCanNextCombo = false;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> WB_SkillCooldownClass;

    UPROPERTY()
    UUserWidget* CooldownWidget = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Targeting")
    AEnemyBase* LockOnTarget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float LockOnRange = 1200.f;

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
};