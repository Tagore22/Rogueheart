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

    // ���� ���� �Լ�
    void SetPlayerState(EPlayerState NewState);
    bool IsDodging() const { return CurrentState == EPlayerState::Dodging; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // �Է� ó�� �Լ���
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    void Dodge(const FInputActionValue& Value);

    // ��ų ��� �Լ�
    UFUNCTION()
        void UseFireball();

    UFUNCTION()
        void UseIceBlast();

    // ���� üũ �Լ�
    bool CanAct() const;

private:
    // ī�޶� ȸ�� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Camera")
        float BaseTurnRate;

    UPROPERTY(EditAnywhere, Category = "Camera")
        float BaseLookUpRate;

    // ī�޶� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCamera;

    // ĳ���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
        EPlayerState CurrentState;

    // �Է� ����
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

    // ���� / ȸ�� �ִϸ��̼�
    UPROPERTY(EditDefaultsOnly, Category = "Attack")
        UAnimMontage* AMT_Attack;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        float DodgeDistance = 600.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        float DodgeSpeed = 2400.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dodge")
        UAnimMontage* AMT_Dodge;

    // ��ų �ý���
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
        USkillComponent* SkillComponent;

    // UI
    UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> WB_SkillCooldownClass;

    UUserWidget* CooldownWidget;
};