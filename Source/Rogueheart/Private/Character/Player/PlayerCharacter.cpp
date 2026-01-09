#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/PlayerAnimInstance.h"
#include "Skill/SkillCooldownWidget.h"
#include "Character/Enemy/EnemyBase.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

    SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    LockOnBreakDistanceSq = FMath::Square(LockOnBreakDistance);
    SetGenericTeamId(FGenericTeamId(TeamID));
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
        EnhancedInput->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
        EnhancedInput->BindAction(IA_Skill1, ETriggerEvent::Started, this, &APlayerCharacter::UseFireball);
        EnhancedInput->BindAction(IA_Skill2, ETriggerEvent::Started, this, &APlayerCharacter::UseIceBlast);
        EnhancedInput->BindAction(IA_LockOn, ETriggerEvent::Started, this, &APlayerCharacter::ToggleLockOn);
        EnhancedInput->BindAction(IA_SwitchTargetLeft, ETriggerEvent::Started, this, &APlayerCharacter::SwitchTargetLeft);
        EnhancedInput->BindAction(IA_SwitchTargetRight, ETriggerEvent::Started, this, &APlayerCharacter::SwitchTargetRight);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsValid(LockOnTarget))
    {
        UpdateLockOnRotation(DeltaTime);
        CheckLockOnDistance();
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector2D = Value.Get<FVector2D>();
    LastMoveInput = FVector(MovementVector2D.X, MovementVector2D.Y, 0.f);

    if (!CanAct() || MovementVector2D.IsNearlyZero())
        return;

    if (Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector2D.X);
        AddMovementInput(RightDirection, MovementVector2D.Y);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    if (IsValid(LockOnTarget))
        return;

    const FVector2D LookAxis = Value.Get<FVector2D>();

    if (LookAxis.IsNearlyZero())
        return;

    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
    if (CurrentState == EPlayerState::Dodging || CurrentState == EPlayerState::Stunned)
        return;

    if (CurrentState != EPlayerState::Attacking)
    {
        CurrentCombo = 1;
        bInputCombo = false;
        // 공격 애니메이션 도중 NextCombo 노티파이에 의해서만 true로 바뀜.
        bCanNextCombo = false; 

        SetPlayerState(EPlayerState::Attacking);
        PlayComboMontage();
    }
    else if (bCanNextCombo && !bInputCombo)
    {
        bInputCombo = true;
    }
} 

void APlayerCharacter::PlayComboMontage()
{
    UPlayerAnimInstance* Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    if (Anim && AMT_Attack)
    {
        Anim->Montage_Play(AMT_Attack);
        Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo)), AMT_Attack);
    }
}

void APlayerCharacter::HandleComboInput()
{
    ++CurrentCombo;
    bInputCombo = false;
    bCanNextCombo = false;

    PlayComboMontage();
}

void APlayerCharacter::OnAttackEnd()
{
    if (bInputCombo && CurrentCombo < MaxCombo)
    {
        HandleComboInput();
    }
    else
    {
        CurrentCombo = 0;
        bInputCombo = false;
        bCanNextCombo = false;
        SetPlayerState(EPlayerState::Idle);
    }
}

// 토클 락온 하는중.
void APlayerCharacter::RestoreLockOnIfNeeded()
{
    if (bWasLockedOnWhenDodged)
    {
        LockOnTarget = FindNearestTarget();

        if (IsValid(LockOnTarget))
        {
            GetCharacterMovement()->bOrientRotationToMovement = false;
            bUseControllerRotationYaw = true;
        }
        bWasLockedOnWhenDodged = false;
    }
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    if (!CanAct() || AMT_Dodge == nullptr)
        return;

    SetPlayerState(EPlayerState::Dodging);
    bWasLockedOnWhenDodged = IsValid(LockOnTarget) ? true : false;

    if (!LastMoveInput.IsNearlyZero())
    {
        FRotator ControlRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
        FVector DodgeDir = ControlRot.RotateVector(LastMoveInput);
        SetActorRotation(DodgeDir.Rotation());
    }

    if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
    {
        Anim->Montage_Play(AMT_Dodge);
    }

    if(IsValid(LockOnTarget))
    {
        LockOnTarget = nullptr;
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
    }
}

void APlayerCharacter::UseFireball()
{
    if (!CanAct()) return;
    SkillComponent->UseSkill(ESkillType::Fireball);
}

void APlayerCharacter::UseIceBlast()
{
    if (!CanAct()) return;
    SkillComponent->UseSkill(ESkillType::IceNova);
}

void APlayerCharacter::SetPlayerState(EPlayerState NewState)
{
    CurrentState = NewState;
}

bool APlayerCharacter::CanAct() const
{
    return CurrentState != EPlayerState::Attacking && CurrentState != EPlayerState::Dodging && CurrentState != EPlayerState::Stunned;
}

void APlayerCharacter::ToggleLockOn()
{
    if (!CanAct())
        return;

    if (IsValid(LockOnTarget))
    {
        ClearLockOn();
    }
    else
    {
        LockOnTarget = FindNearestTarget();

        if (IsValid(LockOnTarget))
        {
            LockOnTarget->ShowTargetMarker(true);

            GetCharacterMovement()->bOrientRotationToMovement = false;
            bUseControllerRotationYaw = true;
        }
    }
}

AEnemyBase* APlayerCharacter::FindNearestTarget()
{
    // 초기화
    //LockOnTarget = nullptr;

    // 1단계: 주변 적들 긁어모으기 (Wide Overlap)
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 나 자신 제외

    // 구체(Sphere) 범위를 생성하여 해당 채널(예: ECC_Pawn)의 물체를 탐색
    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Pawn, // 적의 콜리전 채널에 맞게 변경 가능
        FCollisionShape::MakeSphere(LockOnRange),
        QueryParams
    );

    if (!bHit) 
        return nullptr;

    // 후보군을 담을 구조체 리스트
    struct FTargetCandidate {
        AActor* Actor;
        float DistSq;
    };
    TArray<FTargetCandidate> Candidates;

    FVector CameraLocation = FollowCamera->GetComponentLocation(); // 또는 GetCameraLocation()
    FVector CameraForward = FollowCamera->GetForwardVector();

    // 2단계: 시야각(FOV) 및 유효성 필터링
    for (auto& Result : OverlapResults)
    {
        AEnemyBase* Enemy = Cast<AEnemyBase>(Result.GetActor());
        if (!IsValid(Enemy)) 
            continue;

        FVector ToEnemy = (Enemy->GetActorLocation() - CameraLocation).GetSafeNormal();

        // 내적(Dot Product) 계산
        float DotResult = FVector::DotProduct(CameraForward, ToEnemy);

        // 기준값 (예: 0.5는 약 60도, 0.7은 약 45도 시야 내)
        if (DotResult > 0.5f)
        {
            float DistSq = FVector::DistSquared(CameraLocation, Enemy->GetActorLocation());
            Candidates.Add({ Enemy, DistSq });
        }
    }

    // 3단계: 거리순 정렬 (람다식 활용)
    Candidates.Sort([](const FTargetCandidate& A, const FTargetCandidate& B) {
        return A.DistSq < B.DistSq;
        });

    // 4단계: 최종 시야 검증 (Line Trace)
    for (auto& Candidate : Candidates)
    {
        FHitResult HitResult;
        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(this);

        FVector TraceStart = CameraLocation;
        FVector TraceEnd = Candidate.Actor->GetActorLocation() + FVector(0, 0, 50.0f);

        // 내 눈(카메라)에서 적의 위치(보통 가슴 높이 고려)까지 발사
        bool bIsObstructed = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility, // 장애물 체크용 채널
            TraceParams
        );

        // 아무것도 안 걸렸거나(하늘 등), 걸린 게 바로 그 적이라면 타겟 확정!
        if (!bIsObstructed || HitResult.GetActor() == Candidate.Actor)
        {
            return Cast<AEnemyBase>(Candidate.Actor);
        }
    }
    return nullptr;
}

void APlayerCharacter::UpdateLockOnRotation(float DeltaTime)
{
    if (!LockOnTarget) 
        return;

    FVector TargetDir = LockOnTarget->GetActorLocation() - GetActorLocation();
    TargetDir.Z = 0.f;
    FRotator TargetRot = TargetDir.Rotation();

    FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 5.f);
    SetActorRotation(NewRot);

    if (Controller)
    {
        Controller->SetControlRotation(NewRot);
    }
}

AEnemyBase* APlayerCharacter::SwitchTarget(bool bLeft)
{
    if (!IsValid(LockOnTarget))
        return nullptr;

    FVector MyLocation = GetActorLocation();
    FVector MyForward = GetActorForwardVector();
    FVector MyRight = GetActorRightVector();

    float MinAngle = 180.f;
    AEnemyBase* NewTarget = nullptr;

    for (TActorIterator<AEnemyBase> It(GetWorld()); It; ++It)
    {
        AEnemyBase* Enemy = *It;
        if (!IsValid(Enemy) || Enemy == LockOnTarget)
            continue;

        FVector ToEnemy = Enemy->GetActorLocation() - MyLocation;
        ToEnemy.Z = 0.f;
        ToEnemy.Normalize();

        float DotRight = FVector::DotProduct(ToEnemy, MyRight);
        float DotForward = FVector::DotProduct(ToEnemy, MyForward);

        if (DotForward < 0.3f)
            continue;

        if ((bLeft && DotRight < 0.f) || (!bLeft && DotRight > 0.f))
        {
            float Angle = FMath::Acos(FVector::DotProduct(MyForward, ToEnemy)) * (180.f / PI);
            if (Angle < MinAngle)
            {
                MinAngle = Angle;
                NewTarget = Enemy;
            }
        }
    }
    if (NewTarget)
    {
        if (IsValid(LockOnTarget)) // clear();
        {
            LockOnTarget->ShowTargetMarker(false);
        }
        return NewTarget;
    }
    return nullptr;
}

void APlayerCharacter::SwitchTargetLeft()
{
    if (IsAttacking() || IsDodging())
        return;

    LockOnTarget = SwitchTarget(true);
    LockOnTarget->ShowTargetMarker(true);
}

void APlayerCharacter::SwitchTargetRight()
{
    if (IsAttacking() || IsDodging())
        return;

    LockOnTarget = SwitchTarget(false);
    LockOnTarget->ShowTargetMarker(true);
}

void APlayerCharacter::CheckLockOnDistance()
{
    if (!IsValid(LockOnTarget))
        return;

    float Dist = FVector::DistSquared(GetActorLocation(), LockOnTarget->GetActorLocation());
    if (Dist > LockOnBreakDistanceSq)
    {
        ClearLockOn();
    }
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
    return FGenericTeamId(TeamID);
}

void APlayerCharacter::ClearLockOn()
{
    LockOnTarget->ShowTargetMarker(false);
    LockOnTarget = nullptr;

    // 이동 모드 복구: 가고자 하는 방향으로 몸을 돌림
    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}