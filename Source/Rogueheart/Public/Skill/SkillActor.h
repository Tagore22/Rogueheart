#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/SkillDataRow.h"
#include "SkillActor.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS()
class ROGUEHEART_API ASkillActor : public AActor
{
    GENERATED_BODY()

public:
    ASkillActor();

    // 스킬 속성, 소유자, 데미지 초기화 → 이후 자동으로 ActivateSkill() 호출 
    void InitializeSkill(const FSkillDataRow& SkillData, AActor* SkillOwner, float Damage);

protected:
    virtual void BeginPlay() override;

    // 스킬 실행 진입점 
    virtual void ActivateSkill();

    // 충돌 처리 콜백 
    UFUNCTION()
        void OnSkillOverlap(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);

    // 폭발 처리 
    void Explode();

    // 스킬별 구현 함수 
    void Activate_Fireball();
    void Activate_IceNova();
    void Activate_ThunderStrike();
    void Activate_DashSlash();
    void Activate_HealingLight();

protected:
    // 충돌 컴포넌트 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    USphereComponent* Collision;

    // 이펙트 컴포넌트 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    UNiagaraComponent* SkillEffect;

    // 투사체 이동 컴포넌트 (Fireball, DashSlash) 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    UProjectileMovementComponent* ProjectileMovement;

    // DataTable에서 전송된 현재 스킬 정보 
    FSkillDataRow CurrentSkillData;

    // 실제로 적용할 스킬 데미지 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    float SkillDamage;

    // 스킬 시전자 
    AActor* Caster;
};