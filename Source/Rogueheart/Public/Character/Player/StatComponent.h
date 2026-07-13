/*#pragma once

#include "CoreMinimal.h"
#include "Character/Player/PlayerBaseComponent.h"
#include "StatComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8 // s
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
enum class EActionType : uint8 // s
{
    Move,
    Attack,
    LockOn,
    Dodge,
    Parry,
    UseSkill
};

UCLASS()
class ROGUEHEART_API UStatComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
    void SetPlayerState(EPlayerState NewState); //

    bool CanAct(EActionType DesiredAction) const; // s

    void HealPlayer(float PlusHP); // 

    void OnActStart(); //

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxHP() const { return MaxHP }; // 

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurHP() const { return CurHP };// 

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMaxStamina() const { return MaxStamina };// 

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetCurStamina() const { return CurStamina };// 


protected:
    void ConsumeStamina(float Cost); //

    bool CanPlusStamina() const; //
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    EPlayerState CurrentState = EPlayerState::Idle;

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
};*/
