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

    // AI Perception ����
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*AIPerception);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 800.f;
        SightConfig->LoseSightRadius = 900.f;
        SightConfig->PeripheralVisionAngleDegrees = 60.f;
        SightConfig->SetMaxAge(4.0f);  // 0.5f���� 4.0f�� ����
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.f;

        AIPerception->ConfigureSense(*SightConfig);
        AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
    }

    // �� ����
    SetGenericTeamId(FGenericTeamId(EnemyTeamId));
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // Perception �̺�Ʈ ���ε�
    if (AIPerception)
    {
        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
    }

    // �����̺�� Ʈ�� ����
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
    // TeamId�� ���� �÷��̾� Ȯ��
    FGenericTeamId ActorTeamId = FGenericTeamId::GetTeamIdentifier(Actor);
    if (ActorTeamId != FGenericTeamId(1)) // �÷��̾� TeamId�� 1�� �ƴϸ� ����
        return;

    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB)
        return;

    if (Stimulus.WasSuccessfullySensed())
    {
        // �÷��̾� �߰�
        BB->SetValueAsObject(TEXT("TargetPlayer"), Actor);

        // �߰��� ��ġ ����
        if (APawn* ControlledPawn = GetPawn())
        {
            BB->SetValueAsVector(TEXT("DiscoveredLocation"), ControlledPawn->GetActorLocation());
        }

        UE_LOG(LogTemp, Warning, TEXT("Player Detected! TeamId: %d"), ActorTeamId.GetId());
    }
    else if (Actor == BB->GetValueAsObject(TEXT("TargetPlayer")))
    {
        // �÷��̾� ��ħ
        BB->ClearValue(TEXT("TargetPlayer"));

        UE_LOG(LogTemp, Warning, TEXT("Player Lost! TeamId: %d"), ActorTeamId.GetId());
    }
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
    return FGenericTeamId(EnemyTeamId);
}