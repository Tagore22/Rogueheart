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

    // ��ų �Ӽ�, ������, ������ �ʱ�ȭ �� ���� �ڵ����� ActivateSkill() ȣ�� 
    void InitializeSkill(const FSkillDataRow& SkillData, AActor* SkillOwner, float Damage);

protected:
    virtual void BeginPlay() override;

    // ��ų ���� ������ 
    virtual void ActivateSkill();

    // �浹 ó�� �ݹ� 
    UFUNCTION()
        void OnSkillOverlap(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);

    // ���� ó�� 
    void Explode();

    // ��ų�� ���� �Լ� 
    void Activate_Fireball();
    void Activate_IceNova();
    void Activate_ThunderStrike();
    void Activate_DashSlash();
    void Activate_HealingLight();

protected:
    // �浹 ������Ʈ 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    USphereComponent* Collision;

    // ����Ʈ ������Ʈ 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    UNiagaraComponent* SkillEffect;

    // ����ü �̵� ������Ʈ (Fireball, DashSlash) 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    UProjectileMovementComponent* ProjectileMovement;

    // DataTable���� ���۵� ���� ��ų ���� 
    FSkillDataRow CurrentSkillData;

    // ������ ������ ��ų ������ 
    UPROPERTY(VisibleAnywhere, Category = "Skill")
    float SkillDamage;

    // ��ų ������ 
    AActor* Caster;
};