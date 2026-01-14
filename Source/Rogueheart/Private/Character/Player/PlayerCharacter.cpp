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
#include "Skill/SkillComponent.h"
#include "InputActionValue.h"

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

    if (!CanAct(EActionType::Move) || MovementVector2D.IsNearlyZero())
        return;

    if (IsValid(Controller))
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

void APlayerCharacter::RestoreLockOnIfNeeded()
{
    if (IsValid(PrevLockOnTarget))
    {
        LockOnTarget = PrevLockOnTarget;
        SetLockOnState(true);
    }
    else
    {
        LockOnTarget = FindNearestTarget();

        if (IsValid(LockOnTarget))
        {
            SetLockOnState(true);
        }
    }
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    // 1. 필수 요소가 하나라도 없으면 아예 구르기를 시작조차 안 함
    UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

    if (!CanAct(EActionType::Dodge) || !IsValid(AMT_Dodge) || !Anim)
        return;

    SetPlayerState(EPlayerState::Dodging);

    if (!LastMoveInput.IsNearlyZero())
    {
        FRotator ControlRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
        FVector DodgeDir = ControlRot.RotateVector(LastMoveInput);
        SetActorRotation(DodgeDir.Rotation());
    }

    Anim->Montage_Play(AMT_Dodge);

    if(IsValid(LockOnTarget))
    {
        PrevLockOnTarget = LockOnTarget;
        LockOnTarget = nullptr;
        SetLockOnState(false);
    }
}

void APlayerCharacter::UseFireball()
{
    if (!CanAct(EActionType::UseSkill))
        return;

    SkillComponent->UseSkill(ESkillType::Fireball);
}

void APlayerCharacter::UseIceBlast()
{
    if (!CanAct(EActionType::UseSkill))
        return;

    SkillComponent->UseSkill(ESkillType::IceNova);
}

void APlayerCharacter::SetPlayerState(EPlayerState NewState)
{
    CurrentState = NewState;
}

bool APlayerCharacter::CanAct(EActionType DesiredAction) const
{
    switch (CurrentState)
    {
        case EPlayerState::Idle:
        case EPlayerState::Moving:
            return true;
        case EPlayerState::Attacking:
        {
            if (DesiredAction == EActionType::Attack)
            {
                return true;
            }
            return false;
        }
        case EPlayerState::Dodging:
        case EPlayerState::Stunned:
        case EPlayerState::Dead:
            return false;
    }
    return false;
}

void APlayerCharacter::ToggleLockOn()
{
    if (!CanAct(EActionType::LockOn))
        return;

    if (IsValid(LockOnTarget))
    {
        ClearLockOn();
    }
    else
    {
        AEnemyBase * NewTarget = FindNearestTarget();

        if (IsValid(NewTarget))
        {
            PrevLockOnTarget = nullptr;
            LockOnTarget = NewTarget;
            LockOnTarget->ShowTargetMarker(true);
            SetLockOnState(true);
        }
    }
}

AEnemyBase* APlayerCharacter::FindNearestTarget()
{
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
    if (!IsValid(LockOnTarget)) 
        return;

    FVector TargetDir = LockOnTarget->GetActorLocation() - GetActorLocation();
    TargetDir.Z = 0.f;

    if (TargetDir.IsNearlyZero()) 
        return;

    FRotator TargetRot = TargetDir.Rotation();

    FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, InterpSpeed);
    SetActorRotation(NewRot);

    if (Controller)
    {
        Controller->SetControlRotation(NewRot);
    }
}

/*AEnemyBase* APlayerCharacter::SwitchTarget(bool bLeft)
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
}*/

AEnemyBase* APlayerCharacter::SwitchTarget(bool bLeft)
{
    if (!IsValid(LockOnTarget)) 
        return nullptr;

    // 1. 범위 내 적들만 필터링, OverlapMultiByObjectType 혹은 OverlapMultiByChannel
    TArray<FOverlapResult> OverlappingActors;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(LockOnRange);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    GetWorld()->OverlapMultiByObjectType(OverlappingActors, GetActorLocation(), FQuat::Identity, FCollisionObjectQueryParams(ECC_Pawn), Sphere, Params);

    FVector MyForward = GetActorForwardVector();
    FVector MyRight = GetActorRightVector();
    FVector MyLocation = GetActorLocation();

    AEnemyBase* BestTarget = nullptr;
    // 에디터에서 설정한 MinAngle을 '최대 탐색 허용치'로 사용
    float ClosestAngle = MinAngle;

    for (const FOverlapResult& Result : OverlappingActors)
    {
        AEnemyBase* Enemy = Cast<AEnemyBase>(Result.GetActor());

        // 유효성, 현재 타겟 제외
        if (!IsValid(Enemy) || Enemy == LockOnTarget) 
            continue;

        FVector ToEnemy = (Enemy->GetActorLocation() - MyLocation).GetSafeNormal2D();

        float DotForward = FVector::DotProduct(ToEnemy, MyForward);
        // 앞쪽 시야 확보. 내적값에 의해 1은 정면, 0.5는 60도, 0은 90도.
        if (DotForward < 0.5f) 
            continue; 

        float DotRight = FVector::DotProduct(ToEnemy, MyRight);

        // 왼쪽/오른쪽 방향성 체크
        if ((bLeft && DotRight < 0.f) || (!bLeft && DotRight > 0.f))
        {
            // Clamp를 통해서 -1 ~ 1안의 값으로 강제함.
            float Angle = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(DotForward, -1.f, 1.f)));
            if (Angle < ClosestAngle)
            {
                ClosestAngle = Angle;
                BestTarget = Enemy;
            }
        }
    }
    if (BestTarget)
    {
        //LockOnTarget->ShowTargetMarker(false);
        return BestTarget; // 리턴 후 호출한 곳에서 LockOnTarget = NewTarget; 을 해주겠죠?
    }
    return nullptr;
}

/*void APlayerCharacter::SwitchTargetLeft()
{
    if (IsAttacking() || IsDodging())
        return;

    if (IsValid(LockOnTarget))
    {
        AEnemyBase* NewTarget = SwitchTarget(true);
        if (IsValid(NewTarget))
        {
            LockOnTarget = NewTarget;
            LockOnTarget->ShowTargetMarker(true);
        }
    }
}*/

void APlayerCharacter::SwitchTargetLeft()
{
    if (!CanAct(EActionType::LockOn) || !IsValid(LockOnTarget))
        return;

    AEnemyBase* NewTarget = SwitchTarget(true);
    if (IsValid(NewTarget) && LockOnTarget != NewTarget)
    {
        LockOnTarget->ShowTargetMarker(false);
        LockOnTarget = NewTarget;
        LockOnTarget->ShowTargetMarker(true);
    }
}

/*void APlayerCharacter::SwitchTargetRight()
{
    if (IsAttacking() || IsDodging())
        return;

    if (IsValid(LockOnTarget))
    {
        AEnemyBase* NewTarget = SwitchTarget(false);
        if (IsValid(NewTarget))
        {
            LockOnTarget = NewTarget;
            LockOnTarget->ShowTargetMarker(true);
        }
    }
}*/

void APlayerCharacter::SwitchTargetRight()
{
    if (!CanAct(EActionType::LockOn) || !IsValid(LockOnTarget))
        return;

    AEnemyBase* NewTarget = SwitchTarget(false);
    if (IsValid(NewTarget) && LockOnTarget != NewTarget)
    {
        LockOnTarget->ShowTargetMarker(false);
        LockOnTarget = NewTarget;
        LockOnTarget->ShowTargetMarker(true);
    }
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
    // 2. (추가 제안) 적이 죽었는지도 여기서 같이 체크하면 좋습니다.
    // 만약 Enemy 클래스에 IsDead() 같은 함수가 있다면:
    /*
    if (LockOnTarget->IsDead())
    {
        ClearLockOn();
    }
    */
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
    return FGenericTeamId(TeamID);
}

void APlayerCharacter::ClearLockOn()
{
    if (!IsValid(LockOnTarget))
        return;

    LockOnTarget->ShowTargetMarker(false);
    LockOnTarget = nullptr;

    // 이동 모드 복구: 가고자 하는 방향으로 몸을 돌림
    SetLockOnState(false);
}

void APlayerCharacter::SetLockOnState(bool bIsLockOn)
{
    if (bIsLockOn)
    {
        GetCharacterMovement()->bOrientRotationToMovement = false;
        bUseControllerRotationYaw = true;
    }
    else
    {
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
    }
}