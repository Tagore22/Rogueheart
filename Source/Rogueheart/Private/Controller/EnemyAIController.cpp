#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*AIPerception);

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

        AIPerception->ConfigureSense(*SightConfig);
        AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
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

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (TargetActor)
    {
        APawn* ControlledPawn = GetPawn();
        if (!ControlledPawn) return;

        float Distance = FVector::Dist(TargetActor->GetActorLocation(), ControlledPawn->GetActorLocation());

        if (Distance <= AttackDistance)
        {
            UE_LOG(LogTemp, Warning, TEXT("EnemyAttack"));
            ACharacter* EnemyCharacter = Cast<ACharacter>(ControlledPawn);
            if (EnemyCharacter)
            {
                UAnimInstance* AnimInstance = EnemyCharacter->GetMesh()->GetAnimInstance();
                if (AnimInstance && AttackMontage && !AnimInstance->Montage_IsPlaying(AttackMontage))
                {
                    //AnimInstance->Montage_Play(AttackMontage);
                }
            }
        }
        else
        {
            MoveToActor(TargetActor, 100.f, true, true, true, 0, true);
        }
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("AIPerception"));
    if (Stimulus.WasSuccessfullySensed())
    {
        TargetActor = Actor;
        UE_LOG(LogTemp, Warning, TEXT("Player Sense : %s"), *Actor->GetName());
    }
    else
    {
        if (TargetActor == Actor)
        {
            TargetActor = nullptr;
            StopMovement();
            UE_LOG(LogTemp, Warning, TEXT("Player Lost : %s"), *Actor->GetName());
        }
    }
}
