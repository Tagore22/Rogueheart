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
    Jumping,
    Parrying,
    Stunned
};

UCLASS()
class ROGUEHEART_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

    // 惑怕 包府
    void SetPlayerState(EPlayerState NewState);
protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void Landed(const FHitResult& Hit) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartJump(const FInputActionValue& Value);
    void StopJump(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    void Dodge(const FInputActionValue& Value);
    UFUNCTION()
        void UseFireball();
    UFUNCTION()
        void UseIceBlast();

    // 惑怕 包府
    bool CanAct() const;

private:
    UPROPERTY(EditAnywhere, Category = "Camera")
        float BaseTurnRate;

    UPROPERTY(EditAnywhere, Category = "Camera")
        float BaseLookUpRate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
        EPlayerState CurrentState;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputAction* IA_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputAction* IA_Look;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputAction* IA_Jump;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputAction* IA_Attack;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputAction* IA_Dodge;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputAction* IA_Skill1;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
        UInputAction* IA_Skill2;

    UPROPERTY(EditDefaultsOnly, Category = "Attack")
        UAnimMontage* AMT_Attack;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        float DodgeDistance = 600.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        float DodgeSpeed = 2400.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        UAnimMontage* AMT_Dodge;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
        USkillComponent* SkillComponent;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> WB_SkillCooldownClass;

    UUserWidget* CooldownWidget;
};