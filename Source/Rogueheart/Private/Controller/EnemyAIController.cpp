#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Character/Player/PlayerCharacter.h" // ���� �÷��̾� ĳ���� ����� ���� �ʿ�
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
        SightConfig->SetMaxAge(0.5f); // ������ ���� ����

        // ���� ���: ���� (�÷��̾ ���̸� ������)
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

        // ������ �� ��ġ �ڵ� ���� ����
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
            StopMovement(); // Ȥ�� �� �ߺ� ȣ�� ����
        }
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // �÷��̾ �����ϵ��� ���͸�
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