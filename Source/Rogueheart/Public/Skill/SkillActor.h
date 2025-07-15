#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/SkillDataRow.h"
#include "SkillActor.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class ROGUEHEART_API ASkillActor : public AActor
{
    GENERATED_BODY()

public:
    ASkillActor();

    /**
     * ��ų �Ӽ� �� �����ڸ� �ʱ�ȭ
     */
    void InitializeSkill(const FSkillDataRow& SkillData, AActor* SkillOwner);

protected:
    virtual void BeginPlay() override;

    /** �浹 ó�� �ݹ� */
    UFUNCTION()
        void OnSkillOverlap(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);

    /** ���� ó�� */
    void Explode();

protected:
    /** �浹 ������Ʈ */
    UPROPERTY(VisibleAnywhere, Category = "Skill")
        USphereComponent* Collision;

    /** ����Ʈ ������Ʈ */
    UPROPERTY(VisibleAnywhere, Category = "Skill")
        UNiagaraComponent* SkillEffect;

    /** ���������̺��� ���۵� ���� ��ų ���� */
    FSkillDataRow CurrentSkillData;

    /** ��ų ������ */
    AActor* Caster;
};