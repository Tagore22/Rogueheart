#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

/*AEnemyAIController::AEnemyAIController()
{
    // Perception ������Ʈ ����
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

    // Sight ���� ����
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    if (SightConfig)
    {
        SightConfig->SightRadius = 1000.f;
        SightConfig->LoseSightRadius = 1200.f;
        SightConfig->PeripheralVisionAngleDegrees = 90.f;
        SightConfig->SetMaxAge(5.f);

        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

        if (AIPerception)
        {
            AIPerception->ConfigureSense(*SightConfig);
            AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
        }
    }
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerception)
    {
        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor) return;

    if (Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogTemp, Warning, TEXT("�÷��̾� �߰�: %s"), *Actor->GetName());
        // �ൿ �߰�
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("�÷��̾� ��ħ: %s"), *Actor->GetName());
        // �ൿ �ߴ�
    }
}*/