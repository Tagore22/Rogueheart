#include "Character/Enemy/EnemyBase.h"
#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Ÿ�� ��Ŀ UI ����
    TargetMarker = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetMarker"));
    TargetMarker->SetupAttachment(RootComponent);
    TargetMarker->SetWidgetSpace(EWidgetSpace::Screen);
    TargetMarker->SetDrawAtDesiredSize(true);
    TargetMarker->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

    static ConstructorHelpers::FClassFinder<UUserWidget> MarkerWidgetClass(TEXT("/Game/Characters/WBP_TargetMarker.WBP_TargetMarker_C"));
    if (MarkerWidgetClass.Succeeded())
    {
        TargetMarker->SetWidgetClass(MarkerWidgetClass.Class);
    }

    TargetMarker->SetVisibility(false);
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    // AIController���� Perception �̺�Ʈ ���ε�
    if (auto AICon = Cast<AEnemyAIController>(GetController()))
    {
        if (AICon->GetPerceptionComponent())
        {
            AICon->GetPerceptionComponent()->OnTargetPerceptionUpdated.AddUniqueDynamic(
                this, &AEnemyBase::OnPerceptionUpdated);
        }
    }
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeSinceLastAttack += DeltaTime;

    // �÷��̾ �������� �������� �ƹ��͵� �� ��
    if (!bHasPerceivedPlayer || !TargetPlayer)
        return;

    const float Distance = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

    if (Distance <= AttackRange)
    {
        TryAttack();
    }
    else
    {
        MoveToPlayer();
    }
}

void AEnemyBase::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor && Stimulus.WasSuccessfullySensed())
    {
        TargetPlayer = Cast<APawn>(Actor);
        bHasPerceivedPlayer = true;
    }
    else if (Actor == TargetPlayer && !Stimulus.WasSuccessfullySensed())
    {
        bHasPerceivedPlayer = false;
        TargetPlayer = nullptr;
    }
}

void AEnemyBase::MoveToPlayer()
{
    if (auto AICon = Cast<AAIController>(GetController()))
    {
        AICon->MoveToActor(TargetPlayer, 100.f);
    }
}

void AEnemyBase::TryAttack()
{
    if (TimeSinceLastAttack >= AttackCooldown && AttackMontage)
    {
        PlayAnimMontage(AttackMontage);
        TimeSinceLastAttack = 0.f;
    }
}

void AEnemyBase::ShowTargetMarker(bool bShow)
{
    if (TargetMarker)
    {
        TargetMarker->SetVisibility(bShow);
    }
}