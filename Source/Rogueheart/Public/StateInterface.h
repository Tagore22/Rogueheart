#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillNames.h"
#include "StateInterface.generated.h"

UINTERFACE(MinimalAPI)
class UStateInterface : public UInterface
{
	GENERATED_BODY()
};

class ROGUEHEART_API IStateInterface
{
	GENERATED_BODY()

public:
    virtual float GetMaxHP() const = 0;

    virtual float GetCurHP() const = 0;

    virtual float GetMaxMana() const = 0;

    virtual float GetCurMana() const = 0;

    virtual float GetMaxStamina() const = 0;

    virtual float GetCurStamina() const = 0;

    virtual int32 GetSoulSum() const = 0;

    virtual void SetMaxHP(float NewMaxHP) = 0;

    virtual void SetCurHP(float NewCurHP) = 0;

    virtual void SetMaxMana(float NewMaxMana) = 0;

    virtual void SetCurMana(float NewCurMana) = 0;

    virtual void SetMaxStamina(float NewMaxStamina) = 0;

    virtual void SetCurStamina(float NewCurStamina) = 0;

    virtual void SetSoulSum(int32 Plus) = 0;

    virtual void SetEffectCom(class UNiagaraSystem* Asset) = 0;

    virtual void ActivateEffectCom(bool bIsOn) = 0;

    virtual void SetPlayerState(enum EPlayerState NewState) = 0;

    virtual void CostMana(float Cost) = 0;
};
