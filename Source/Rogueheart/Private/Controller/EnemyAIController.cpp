#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Character/Player/PlayerCharacter.h" // 너의 플레이어 캐릭터 헤더로 수정 필요
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*AIPerception);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 800.f;
        SightConfig->LoseSightRadius = 900.f;
        SightConfig->PeripheralVisionAngleDegrees = 60.f;
        SightConfig->SetMaxAge(0.5f); // 오래된 감지 무시

        // 감지 대상: 적만 (플레이어가 적이면 감지함)
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

        // 마지막 본 위치 자동 추적 제거
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.f;

        AIPerception->ConfigureSense(*SightConfig);
        AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
    }

    // 팀 설정
    SetGenericTeamId(FGenericTeamId(EnemyTeamId));
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerception)
    {
        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
    }
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (TargetActor)
    {
        const float Distance = FVector::Dist(GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());

        if (Distance > AttackDistance)
        {
            MoveToActor(TargetActor, 100.f);
        }
        else
        {
            StopMovement(); // 혹시 모를 중복 호출 방지
        }
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // 플레이어만 감지하도록 필터링
    if (!Actor->IsA<APlayerCharacter>()) return;

    if (Stimulus.WasSuccessfullySensed())
    {
        TargetActor = Actor;
    }
    else if (TargetActor == Actor)
    {
        TargetActor = nullptr;
        StopMovement();
    }
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
    return FGenericTeamId(EnemyTeamId);
}