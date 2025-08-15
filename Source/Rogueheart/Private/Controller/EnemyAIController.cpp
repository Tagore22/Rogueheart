#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GenericTeamAgentInterface.h"
#include "TimerManager.h" // ADDED
#include "GameFramework/Pawn.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = false;

    // AI Perception 설정
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*AIPerception);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 800.f;
        SightConfig->LoseSightRadius = 900.f;

        // FIX: 시야각을 넓혀 어느 방향으로 이동해도 인식될 가능성 증가
        SightConfig->PeripheralVisionAngleDegrees = 180.f;

        // FIX: 캐시(MaxAge)를 짧게 해서 실시간성 개선
        SightConfig->SetMaxAge(0.5f);

        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
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

    // Perception 이벤트 바인딩
    if (AIPerception)
    {
        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);

        // ADDED: BeginPlay 직후 stimuli 업데이트 요청 (초기 반영 보장)
        AIPerception->RequestStimuliListenerUpdate();
    }

    // 비헤이비어 트리 시작
    if (BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Behavior Tree Asset not found!"));
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor)
        return;

    // TeamId를 통한 플레이어 확인
    FGenericTeamId ActorTeamId = FGenericTeamId::GetTeamIdentifier(Actor);
    if (ActorTeamId != FGenericTeamId(1)) // 플레이어 TeamId가 1이 아니면 무시
        return;

    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB)
        return;

    if (Stimulus.WasSuccessfullySensed())
    {
        // 플레이어 발견
        BB->SetValueAsObject(TEXT("TargetPlayer"), Actor);

        // FIX: 발견 위치는 플레이어(Actor)의 현재 위치로 저장 (이전: ControlledPawn 위치 사용)
        BB->SetValueAsVector(TEXT("DiscoveredLocation"), Actor->GetActorLocation());

        // ADDED: 조사 모드 종료 플래그
        BB->SetValueAsBool(TEXT("IsInvestigating"), false);

        // ADDED: 기존 조사 타이머가 있으면 취소
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(InvestigateTimerHandle);
        }

        UE_LOG(LogTemp, Warning, TEXT("Player Detected! TeamId: %d"), ActorTeamId.GetId());
    }
    else
    {
        // 플레이어 놓침: 현재 블랙보드의 타겟과 일치하거나 타겟이 비어있을 때만 조사 시작
        UObject* CurrentTargetObj = BB->GetValueAsObject(TEXT("TargetPlayer"));
        AActor* CurrentTargetActor = Cast<AActor>(CurrentTargetObj);

        if (CurrentTargetActor == Actor || CurrentTargetActor == nullptr)
        {
            // 마지막 목격 위치로 Actor의 현재 위치 사용 (안전한 기본)
            FVector LastSeenLocation = Actor->GetActorLocation();

            // ADDED: 블랙보드에 저장
            BB->SetValueAsVector(TEXT("DiscoveredLocation"), LastSeenLocation);

            BB->SetValueAsBool(TEXT("IsInvestigating"), true);

            // 타겟 클리어해서 전투 브랜치 진입 방지
            BB->ClearValue(TEXT("TargetPlayer"));

            // ADDED: 조사 타이머 시작 (타이머 만료 시 StopInvestigating 호출)
            if (GetWorld())
            {
                GetWorld()->GetTimerManager().ClearTimer(InvestigateTimerHandle);
                GetWorld()->GetTimerManager().SetTimer(InvestigateTimerHandle, this, &AEnemyAIController::StopInvestigating, InvestigateTimeout, false);
            }

            UE_LOG(LogTemp, Warning, TEXT("Player Lost -> Investigating at %s, TeamId: %d"), *LastSeenLocation.ToString(), ActorTeamId.GetId());
        }
    }
}

void AEnemyAIController::StopInvestigating()
{
    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB)
        return;

    // ADDED: 조사 종료 플래그 및 위치 클리어
    BB->SetValueAsBool(TEXT("IsInvestigating"), false);
    BB->ClearValue(TEXT("DiscoveredLocation"));

    // ADDED: 타이머 클리어 안전 처리
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(InvestigateTimerHandle);
    }

    UE_LOG(LogTemp, Warning, TEXT("Investigation timed out -> Returning to Patrol"));
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
    return FGenericTeamId(EnemyTeamId);
}