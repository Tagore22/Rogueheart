#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GenericTeamAgentInterface.h"

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
        SightConfig->PeripheralVisionAngleDegrees = 60.f;
        SightConfig->SetMaxAge(4.0f);  // 0.5f에서 4.0f로 증가
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

        // 발견한 위치 저장
        if (APawn* ControlledPawn = GetPawn())
        {
            BB->SetValueAsVector(TEXT("DiscoveredLocation"), ControlledPawn->GetActorLocation());
        }

        UE_LOG(LogTemp, Warning, TEXT("Player Detected! TeamId: %d"), ActorTeamId.GetId());
    }
    else if (Actor == BB->GetValueAsObject(TEXT("TargetPlayer")))
    {
        // 플레이어 놓침
        BB->ClearValue(TEXT("TargetPlayer"));

        UE_LOG(LogTemp, Warning, TEXT("Player Lost! TeamId: %d"), ActorTeamId.GetId());
    }
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
    return FGenericTeamId(EnemyTeamId);
}