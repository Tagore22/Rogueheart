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
#include "InventoryComponent.h"
#include "Character/Player/RogueheartPlayerController.h"
#include "Rogueheart.h" // ECC_Enemy를 불러오기 위함.

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

    SetLockOnState(false);
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

    SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

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
        EnhancedInput->BindAction(IA_InventoryOnOff, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventory);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsValid(LockOnTarget))
        return;

    UpdateLockOnRotation(DeltaTime);
    CheckLockOnDistance();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    CachedController = Cast<ARogueheartPlayerController>(NewController);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    if (!CanAct(EActionType::Move))
        return;

    // 지역 변수를 새로 만들 땐 const를 이용하여 의도의 명확성을 추가함.
    // value.Get<>()에서 캐스팅되는 타입은 에디터에서 해당 InputAction에 설정한
    // 값에 따라 다르다. Axis1D = float, Axis2D = FVector2D, Axis3D = FVector이다.
    const FVector2D MovementVector2D = Value.Get<FVector2D>();
    if (MovementVector2D.IsNearlyZero())
        return;

    LastMoveInput = FVector(MovementVector2D.X, MovementVector2D.Y, 0.f);

    const FRotator Rotation = GetControlRotation();
    const FRotationMatrix YawMatrix = FRotator(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = YawMatrix.GetUnitAxis(EAxis::X);
    const FVector RightDirection = YawMatrix.GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementVector2D.X);
    AddMovementInput(RightDirection, MovementVector2D.Y);
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
    if (!CanAct(EActionType::Attack))
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
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();
    if (!Anim || !AMT_Attack)
        return;

    Anim->Montage_Play(AMT_Attack);
    Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo)), AMT_Attack);
}

// 공격 애니메이션의 재생이 끝나면 호출되는 노티파이인
// UPlayerAnimInstance::AnimNotify_EndAttack()에서 이 함수를 호출한다.
void APlayerCharacter::OnAttackEnd()
{
    if (bInputCombo && CurrentCombo < MaxCombo)
    {
        ++CurrentCombo;
        PlayComboMontage();
    }
    else
    {
        CurrentCombo = 0;
        SetPlayerState(EPlayerState::Idle);
    }
    bInputCombo = false;
    bCanNextCombo = false;
}

// Dodge 애니메이션의 재생이 끝나면 호출되는 노티파이인
// UPlayerAnimInstance::AnimNotify_EndDodge()에서 이 함수를 호출한다.
// 락온 상태에서 구르기 행동시에만 발동하여 PrevLockOnTarget의 처리를 담당한다.
void APlayerCharacter::RestoreLockOnIfNeeded()
{
    if (!IsValid(PrevLockOnTarget))
        return;
    
    LockOnTarget = PrevLockOnTarget;
    SetLockOnState(true);
    PrevLockOnTarget = nullptr;
}

// 여기부터.
void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();

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

void APlayerCharacter::UseFireball(const FInputActionValue& Value)
{
    if (!CanAct(EActionType::UseSkill))
        return;

    SkillComponent->UseSkill(ESkillType::Fireball);
}

void APlayerCharacter::UseIceBlast(const FInputActionValue& Value)
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

void APlayerCharacter::ToggleLockOn(const FInputActionValue& Value)
{
    if (!CanAct(EActionType::LockOn))
        return;

    if (IsValid(LockOnTarget))
    {
        ClearLockOn();
        //PrevLockOnTarget = nullptr;
    }
    else
    {
        AEnemyBase * NewTarget = FindNearestTarget();

        if (IsValid(NewTarget))
        {
            LockOnTarget = NewTarget;
            LockOnTarget->ShowTargetMarker(true);
            SetLockOnState(true);
        }
    }
}

/*AEnemyBase* APlayerCharacter::FindNearestTarget()
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
        // Get함수를 사용하고 있으나 Result는 이 클래스의 액터와 생명주기가 같다는 보장이
        // 결코 없으니 단순 nullptr 검사 이외에 반드시 IsValid()를 통해 파괴에 대한 안전성
        // 검사를 해야만 한다.
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
        FVector TraceEnd = Candidate.Actor->GetActorLocation() + FVector(0, 0, 50.f);

        // 내 눈(카메라)에서 적의 위치(보통 가슴 높이 고려)까지 발사
        bool bIsObstructed = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility, // 장애물 체크용 채널
            TraceParams
        );

        // 아무것도 안 걸렸거나(하늘 등), 걸린 게 바로 그 적이라면 타겟 확정!
        if (!bIs    Obstructed || HitResult.GetActor() == Candidate.Actor)
        {
            return Cast<AEnemyBase>(Candidate.Actor);
        }
    }
    return nullptr;
}*/

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
        ECC_Enemy, // 적의 콜리전 채널에 맞게 변경 가능. 다만 충돌 반응을 반드시 block으로 할 것.
                   // OnComponentBeginOverlap()과 같이 무조건 overlap이 아닌 경우에는 block을 쓸 것.
        FCollisionShape::MakeSphere(LockOnRange),
        QueryParams
    );

    if (!bHit)
        return nullptr;

    TArray<TPair<float, AEnemyBase*>> Candidates;
    FVector CameraLocation = FollowCamera->GetComponentLocation();
    FVector CameraForward = FollowCamera->GetForwardVector();

    // 2단계: 시야각(FOV) 및 유효성 필터링
    for (const FOverlapResult& Result : OverlapResults)
    {
        AEnemyBase* Enemy = Cast<AEnemyBase>(Result.GetActor());
        // Get함수를 사용하고 있으나 Result는 이 클래스의 액터와 생명주기가 같다는 보장이
        // 결코 없으니 단순 nullptr 검사 이외에 반드시 IsValid()를 통해 파괴에 대한 안전성
        // 검사를 해야만 한다.
        if (!IsValid(Enemy))
            continue;

        FVector ToEnemy = (Enemy->GetActorLocation() - CameraLocation).GetSafeNormal2D();

        // 내적(Dot Product) 계산. 1은 두 벡터가 같은 방향, 0은 수직, -1은 반대방향.
        float DotResult = CameraForward.Dot(ToEnemy);

        // 기준값 (예: 0.5는 약 60도, 0.7은 약 45도 시야 내)
        if (DotResult >= TargetingAngle)
        {
            float DistSq = FVector::DistSquared(CameraLocation, Enemy->GetActorLocation());
            Candidates.Add({ DistSq, Enemy });
        }
    }

    // 3단계: 거리순 정렬 (람다식 활용)
    Candidates.Sort();

    // 4단계: 최종 시야 검증 (Line Trace)
    for (auto& Candidate : Candidates)
    {
        FHitResult HitResult;
        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(this);

        FVector TraceStart = CameraLocation;
        FVector TraceEnd = Candidate.Value->GetActorLocation() + FVector(0.f, 0.f, 50.f);

        // 내 눈(카메라)에서 적의 위치(보통 가슴 높이 고려)까지 발사
        bool bIsObstructed = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility, // 장애물 체크용 채널
            TraceParams
        );

        // 적보다 높게 쏴서 아무것도 안 걸렸거나, 걸린 게 바로 그 적이라면 타겟 확정!
        if (!bIsObstructed || HitResult.GetActor() == Candidate.Value)
        {
            return Candidate.Value;
        }
    }
    return nullptr;
}

void APlayerCharacter::UpdateLockOnRotation(float DeltaTime)
{
    if (!IsValid(LockOnTarget)) 
        return;

    FVector CameraDir = LockOnTarget->GetActorLocation() - GetActorLocation();
    if (CameraDir.IsNearlyZero())
        return;

    FVector TargetDir = FVector(CameraDir.X, CameraDir.Y, 0.f);
    FRotator NewActorRot = FMath::RInterpTo(GetActorRotation(), TargetDir.Rotation(), DeltaTime, InterpSpeed);
    SetActorRotation(NewActorRot);

    if (!CachedController)
        return;

    FRotator NewCameraRot = FMath::RInterpTo(GetControlRotation(), CameraDir.Rotation(), DeltaTime, InterpSpeed);
    CachedController->SetControlRotation(NewCameraRot);
}

AEnemyBase* APlayerCharacter::SwitchTarget(bool bLeft)
{
    if (!IsValid(LockOnTarget)) 
        return nullptr;

    // 1. 범위 내 적들만 필터링, OverlapMultiByObjectType 혹은 OverlapMultiByChannel
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 나 자신 제외

    // 구체(Sphere) 범위를 생성하여 해당 채널(예: ECC_Pawn)의 물체를 탐색
    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Enemy, // 적의 콜리전 채널에 맞게 변경 가능. 다만 충돌 반응을 반드시 block으로 할 것.
                   // OnComponentBeginOverlap()과 같이 무조건 overlap이 아닌 경우에는 block을 쓸 것.
        FCollisionShape::MakeSphere(LockOnRange),
        QueryParams
    );

    if (!bHit)
        return nullptr;

    FVector MyLocation = GetActorLocation();
    FVector MyForward = (LockOnTarget->GetActorLocation() - MyLocation).GetSafeNormal2D();
    FVector MyRight = FVector::UpVector.Cross(MyForward);


    AEnemyBase* BestTarget = nullptr;
    // 에디터에서 설정한 TargetingAngle을 '최대 탐색 허용치'로 사용
    float ClosestAngle = TargetingAngle;

    for (const FOverlapResult& Result : OverlapResults)
    {
        AEnemyBase* Enemy = Cast<AEnemyBase>(Result.GetActor());

        // 유효성, 현재 타겟 제외
        // FindNearestTarget()에도 상술하였듯이 Enemy는 나와 생명주기가 같다고
        // 보장되지 않은 액터 Result의 Get함수이므로 반드시 IsValid()를 통해
        // 안정성 검사를 해야한다.
        if (!IsValid(Enemy) || Enemy == LockOnTarget) 
            continue;

        FVector ToEnemy = (Enemy->GetActorLocation() - MyLocation).GetSafeNormal2D();

        float DotForward = MyForward.Dot(ToEnemy);
        // 앞쪽 시야 확보. 내적값에 의해 1은 정면, 0.5는 60도, 0은 90도.
        if (DotForward < TargetingAngle) 
            continue; 

        float DotRight = MyRight.Dot(ToEnemy);

        // 왼쪽/오른쪽 방향성 체크
        if ((bLeft && DotRight < 0.f) || (!bLeft && DotRight > 0.f))
        {
            if (DotForward > ClosestAngle)
            {
                ClosestAngle = DotForward;
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

void APlayerCharacter::SwitchTargetLeft(const FInputActionValue& Value)
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

void APlayerCharacter::SwitchTargetRight(const FInputActionValue& Value)
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
    // 위 코드를 추가할 시 따로 if문을 만들지 말고 or 연산으로 위 if문에 추가할 것.
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

/*void APlayerCharacter::ToggleInventory(const struct FInputActionValue& Value)
{
    // 인벤토리가 켜져있으면 끄고, 꺼져있으면 킨다.
    // 아마 현재 UI가 nullptr인지로 알 수 있다.
    UE_LOG(LogTemp, Warning, TEXT("Inventory Toggle"));
    //UE_LOG(LogTemp, Log, TEXT("LockOnTarget: nullptr"));

    if (!InventoryWidget)
        return;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC)
        return;

    // 현재 인벤토리가 켜져 있음.
    if (InventoryWidget->IsInViewport())
    {
        InventoryWidget->RemoveFromParent();

        // 마우스 커서 숨기고 게임 입력으로 전환
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
        PC->SetPause(false);
    }
    else
    {
        InventoryWidget->AddToViewport();

        // 마우스 커서 보이고 UI 입력으로 전환
        FInputModeGameAndUI InputMode;
        // 마우스 클릭 시 커서가 갑자기 사라지는 걸 방지
        InputMode.SetHideCursorDuringCapture(false);
        InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
        PC->SetPause(true);
    }
}*/

void APlayerCharacter::ToggleInventory(const FInputActionValue& Value)
{
    // 전부 UI가 있는 컨트롤러로 옮겨졌음.
    if (!CachedController)
        return;

    CachedController->ToggleInventory();
}

float APlayerCharacter::GetMaxHP() const
{
    return MaxHP;
}

float APlayerCharacter::GetCurHP() const
{
    return CurHP;
}

float APlayerCharacter::GetMaxStamina() const
{
    return MaxStamina;
}

float APlayerCharacter::GetCurStamina() const
{
    return CurStamina;
}

void APlayerCharacter::HealPlayer(float PlusHP)
{
    UE_LOG(LogTemp, Warning, TEXT("PrevHP : %f"), CurHP);
    CurHP = FMath::Min(CurHP + PlusHP, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), CurHP);
}