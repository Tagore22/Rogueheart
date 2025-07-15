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
     * 스킬 속성 및 소유자를 초기화
     */
    void InitializeSkill(const FSkillDataRow& SkillData, AActor* SkillOwner);

protected:
    virtual void BeginPlay() override;

    /** 충돌 처리 콜백 */
    UFUNCTION()
        void OnSkillOverlap(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);

    /** 폭발 처리 */
    void Explode();

protected:
    /** 충돌 컴포넌트 */
    UPROPERTY(VisibleAnywhere, Category = "Skill")
        USphereComponent* Collision;

    /** 이펙트 컴포넌트 */
    UPROPERTY(VisibleAnywhere, Category = "Skill")
        UNiagaraComponent* SkillEffect;

    /** 데이터테이블에서 전송된 현재 스킬 정보 */
    FSkillDataRow CurrentSkillData;

    /** 스킬 시전자 */
    AActor* Caster;
};