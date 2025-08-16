#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GenericTeamAgentInterface.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
//
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true; 

    // AI Perception ����
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*AIPerception);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 800.f;
        SightConfig->LoseSightRadius = 900.f;

        // FIX: �þ߰��� ���� ��� �������� �̵��ص� �νĵ� ���ɼ� ����
        SightConfig->PeripheralVisionAngleDegrees = 60.f;

        // FIX: ĳ��(MaxAge)�� ª�� �ؼ� �ǽð��� ����
        SightConfig->SetMaxAge(0.5f);

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

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // �� Tick���� �þ� ����� �׷���
    //Debug_DrawFOV();
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // Perception �̺�Ʈ ���ε�
    if (AIPerception)
    {
        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);

        // ADDED: BeginPlay ���� stimuli ������Ʈ ��û (�ʱ� �ݿ� ����)
        AIPerception->RequestStimuliListenerUpdate();
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
    if (!Actor)
        return;

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

        // FIX: �߰� ��ġ�� �÷��̾�(Actor)�� ���� ��ġ�� ���� (����: ControlledPawn ��ġ ���)
        BB->SetValueAsVector(TEXT("DiscoveredLocation"), Actor->GetActorLocation());

        // ADDED: ���� ��� ���� �÷���
        BB->SetValueAsBool(TEXT("IsInvestigating"), false);

        // ADDED: ���� ���� Ÿ�̸Ӱ� ������ ���
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(InvestigateTimerHandle);
        }

        UE_LOG(LogTemp, Warning, TEXT("Player Detected! TeamId: %d"), ActorTeamId.GetId());
    }
    else
    {
        // �÷��̾� ��ħ: ���� �������� Ÿ�ٰ� ��ġ�ϰų� Ÿ���� ������� ���� ���� ����
        UObject* CurrentTargetObj = BB->GetValueAsObject(TEXT("TargetPlayer"));
        AActor* CurrentTargetActor = Cast<AActor>(CurrentTargetObj);

        if (CurrentTargetActor == Actor || CurrentTargetActor == nullptr)
        {
            // ������ ��� ��ġ�� Actor�� ���� ��ġ ��� (������ �⺻)
            FVector LastSeenLocation = Actor->GetActorLocation();

            // ADDED: �����忡 ����
            BB->SetValueAsVector(TEXT("DiscoveredLocation"), LastSeenLocation);

            BB->SetValueAsBool(TEXT("IsInvestigating"), true);

            // Ÿ�� Ŭ�����ؼ� ���� �귣ġ ���� ����
            BB->ClearValue(TEXT("TargetPlayer"));

            // ADDED: ���� Ÿ�̸� ���� (Ÿ�̸� ���� �� StopInvestigating ȣ��)
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

    // ADDED: ���� ���� �÷��� �� ��ġ Ŭ����
    BB->SetValueAsBool(TEXT("IsInvestigating"), false);
    BB->ClearValue(TEXT("DiscoveredLocation"));

    // ADDED: Ÿ�̸� Ŭ���� ���� ó��
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

void AEnemyAIController::Debug_DrawFOV()
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn || !SightConfig) return;

    FVector PawnLoc = MyPawn->GetActorLocation();
    FVector Forward = MyPawn->GetActorForwardVector();
    float HalfAngle = SightConfig->PeripheralVisionAngleDegrees; // �þ� �ݰ� (��: 90���� �� 180��)
    float Radius = SightConfig->SightRadius;

    // �߽ɼ�
    DrawDebugLine(GetWorld(), PawnLoc, PawnLoc + Forward * Radius, FColor::Green, false, -1.f, 0, 1.f);

    // ���� ���
    FVector LeftDir = Forward.RotateAngleAxis(-HalfAngle, FVector::UpVector);
    DrawDebugLine(GetWorld(), PawnLoc, PawnLoc + LeftDir * Radius, FColor::Green, false, -1.f, 0, 1.f);

    // ������ ���
    FVector RightDir = Forward.RotateAngleAxis(HalfAngle, FVector::UpVector);
    DrawDebugLine(GetWorld(), PawnLoc, PawnLoc + RightDir * Radius, FColor::Green, false, -1.f, 0, 1.f);

    // ��ä�� ����(���� ����)
    for (int i = -HalfAngle; i <= HalfAngle; i += 10) // 10�� �������� ���� �׸�
    {
        FVector Dir = Forward.RotateAngleAxis(i, FVector::UpVector);
        DrawDebugLine(GetWorld(), PawnLoc, PawnLoc + Dir * Radius, FColor(0, 255, 0, 50), false, -1.f, 0, 0.5f);
    }
}