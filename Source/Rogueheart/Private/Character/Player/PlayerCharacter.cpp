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
#include "Rogueheart.h" 
#include "WeaponBase.h"
#include "WeaponSweepComponent.h"
#include "Skill/SkillBaseComponent.h"
#include "Character/Player/MoveComponent.h"
#include "Character/Player/AttackComponent.h"
#include "Character/Player/TargetComponent.h"

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

    //SetLockOnState(false);
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

    SweepCom = CreateDefaultSubobject<UWeaponSweepComponent>(TEXT("SweepComponent"));

    SkillBaseCom = CreateDefaultSubobject<USkillBaseComponent>(TEXT("SKillBaseComponent"));

    MoveCom = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));

    AttackCom = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));

    TargetCom = CreateDefaultSubobject<UTargetComponent>(TEXT("TargetComponent"));
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // LockOnBreakDistanceSq = FMath::Square(LockOnBreakDistance); // t
    SetGenericTeamId(FGenericTeamId(TeamID));
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        //EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move); // m
        //EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look); // m
        //EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::Attack); // a
        //EnhancedInput->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APlayerCharacter::Dodge); // m
        EnhancedInput->BindAction(IA_Skill1, ETriggerEvent::Started, this, &APlayerCharacter::UseFireball);
        EnhancedInput->BindAction(IA_Skill2, ETriggerEvent::Started, this, &APlayerCharacter::UseIceBlast);
        //EnhancedInput->BindAction(IA_LockOn, ETriggerEvent::Started, this, &APlayerCharacter::ToggleLockOn); // t
        //EnhancedInput->BindAction(IA_SwitchTargetLeft, ETriggerEvent::Started, this, &APlayerCharacter::SwitchTargetLeft); // t
        //EnhancedInput->BindAction(IA_SwitchTargetRight, ETriggerEvent::Started, this, &APlayerCharacter::SwitchTargetRight); // t
        EnhancedInput->BindAction(IA_InventoryOnOff, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventory);
        EnhancedInput->BindAction(IA_Skill_Q, ETriggerEvent::Started, this, &APlayerCharacter::UseSkill);
        EnhancedInput->BindAction(IA_Skill_E, ETriggerEvent::Started, this, &APlayerCharacter::UseSkill);
        EnhancedInput->BindAction(IA_Skill_R, ETriggerEvent::Started, this, &APlayerCharacter::UseSkill);

        // 여기서 각 컴포넌트들의 SetupInputBinding()을 호출한다.
        MoveCom->SetupInputBinding(EnhancedInput);
        AttackCom->SetupInputBinding(EnhancedInput);
        TargetCom->SetupInputBinding(EnhancedInput);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /*if (!IsValid(LockOnTarget))
        return;
    
    if (LockOnTarget->GetCurHP() <= 0.f)
    {
        ClearLockOn();
        return;
    }

    UpdateLockOnRotation(DeltaTime);
    CheckLockOnDistance();*/

    // 타겟팅 컴포넌트쪽으로. 그 안에 타겟팅 된 적의 포인터를 가지고 있으며 지금 이 검사 역시
    // 타겟팅 컴포넌트의 Tick에서 실행하여 플레이어의 움직임을 조작한다.
    /*if (IsValid(LockOnTarget))
    {
        if (LockOnTarget->GetCurHP() <= 0.f)
        {
            ClearLockOn(); // 타겟팅으로.
        }
        else
        {
            UpdateLockOnRotation(DeltaTime);
            CheckLockOnDistance();
        }
    }*/

    if (CanPlusStamina()) // StatComponent에서 처리할 부분.
    {
        ConsumeStamina(-PlusStamina);
    }
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    CachedController = Cast<ARogueheartPlayerController>(NewController);
}

/*void APlayerCharacter::Move(const FInputActionValue& Value) // m
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
}*/

/*void APlayerCharacter::Look(const FInputActionValue& Value) // m
{
    if (IsValid(LockOnTarget)) // bool을 반환하는 public 함수로 변경.
        return;

    const FVector2D LookAxis = Value.Get<FVector2D>();
    if (LookAxis.IsNearlyZero())
        return;

    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}*/

/*void APlayerCharacter::Attack(const FInputActionValue& Value) // a
{
    if (!CanAct(EActionType::Attack) || CurStamina <= 0.f) // 스테미나 값을 반환하는 public 함수를 사용.
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
}*/ 

/*void APlayerCharacter::PlayComboMontage() // a
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();

    if (!Anim || AttackMontages.Num() == 0)
        return;

    Anim->Montage_Play(AttackMontages[CurrentCombo - 1]);
}*/

// 공격 애니메이션의 재생이 끝나면 호출되는 노티파이인
// UPlayerAnimInstance::AnimNotify_EndAttack()에서 이 함수를 호출한다.
void APlayerCharacter::OnAttackEnd() // a
{
    /*if (bInputCombo && CurrentCombo < MaxCombo)
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
    */
    // AttackComponent의 함수를 호출하는 래핑함수로 변환.
    AttackCom->OnAttackEnd();
}

// PlayerBaseComponent로 옮겨졌다. PlayerBaseComponent의 RestoreLockOnIfNeeded()를 
// 호출하는 래핑 함수로 변하였다. 애니메이션의 노티파이에서 호출하게 된다.
void APlayerCharacter::RestoreLockOnIfNeeded() // t
{
    /*if (!IsValid(PrevLockOnTarget))
        return;
    
    LockOnTarget = PrevLockOnTarget;
    SetLockOnState(true);
    PrevLockOnTarget = nullptr;*/

    TargetCom->RestoreLockOnIfNeeded();
}

/*void APlayerCharacter::Dodge(const FInputActionValue& Value) // m
{
    // if문의 첫번째는 현재 방향키를 눌렀느냐이다. 사실 이 부분은 뒤로 물러나는 행동이 발동하여야 한다.
    // 구현할 것인가...
    if (LastMoveInput.IsNearlyZero() || !CanAct(EActionType::Dodge) || CurStamina <= 0.f)
        return;

    UAnimInstance* Anim = GetMesh()->GetAnimInstance();
    if (!AMT_Dodge || !Anim)
        return;

    SetWeaponVisible(true);
    // 구르기 이전 해당 방향으로 액터를 회전. 후에 부자연스럽다면 삭제할 것.
    if (!LastMoveInput.IsNearlyZero())
    {
        FRotator ControlRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
        FQuat ControlQuat = ControlRot.Quaternion();
        FVector DodgeDir = ControlQuat.RotateVector(LastMoveInput);
        SetActorRotation(DodgeDir.Rotation());
    }

    SetPlayerState(EPlayerState::Dodging);
    Anim->Montage_Play(AMT_Dodge);

    if(IsValid(LockOnTarget))
    {
        PrevLockOnTarget = LockOnTarget;
        LockOnTarget = nullptr;
        SetLockOnState(false);
    }
}*/

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

// 플레이어의 상태는 바로 이곳에서 관리해야하기에 옮기지 아니한다.
void APlayerCharacter::SetPlayerState(EPlayerState NewState) // s
{
    CurrentState = NewState;
}

// 플레이어의 상태는 바로 이곳에서 관리해야하기에 옮기지 아니한다.
bool APlayerCharacter::CanAct(EActionType DesiredAction) const // s
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
        case EPlayerState::Damaged:
        case EPlayerState::Dead:
            return false;
    }
    return false;
}

/*void APlayerCharacter::ToggleLockOn(const FInputActionValue& Value) // t 
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
            SetLockOnTarget(NewTarget);
        }
    }
}*/

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

/*AEnemyBase* APlayerCharacter::FindNearestTarget() // t
{
    // 1단계: 주변 적들 긁어모으기 (Wide Overlap)
    FVector CameraLocation = FollowCamera->GetComponentLocation();
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 나 자신 제외

    // 구체(Sphere) 범위를 생성하여 해당 채널(예: ECC_Pawn)의 물체를 탐색
    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        CameraLocation,
        FQuat::Identity,
        TraceChannel::ECC_Player, // 적의 콜리전 채널에 맞게 변경 가능. 다만 충돌 반응을 반드시 block으로 할 것.
                             // OnComponentBeginOverlap()과 같이 무조건 overlap이 아닌 경우에는 block을 쓸 것.
        FCollisionShape::MakeSphere(LockOnRange),
        QueryParams
    );

    if (!bHit)
        return nullptr;

    TArray<TPair<float, AEnemyBase*>> Candidates;
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
        float DotResult = FVector::DotProduct(CameraForward, ToEnemy);

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
        bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility, // 장애물 체크용 채널
            TraceParams
        );

        // 적보다 높게 쏴서 아무것도 안 걸렸거나, 걸린 게 바로 그 적이라면 타겟 확정!
        if (!bHit || HitResult.GetActor() == Candidate.Value)
        {
            return Candidate.Value;
        }
    }
    return nullptr;
}*/

/*void APlayerCharacter::UpdateLockOnRotation(float DeltaTime) // t
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
}*/

/*AEnemyBase* APlayerCharacter::SwitchTarget(bool bLeft) // t
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
        TraceChannel::ECC_Player, // 적의 콜리전 채널에 맞게 변경 가능. 다만 충돌 반응을 반드시 block으로 할 것.
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
}*/


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

/*void APlayerCharacter::SwitchTargetLeft(const FInputActionValue& Value)
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
}*/

/*void APlayerCharacter::SwitchTargetLeft(const FInputActionValue& Value) // t
{
    if (!CanAct(EActionType::LockOn) || !IsValid(LockOnTarget))
        return;

    AEnemyBase* Newtarget = SwitchTarget(true);
    if (!IsValid(Newtarget) || Newtarget == LockOnTarget)
        return;

    ClearLockOn();
    SetLockOnTarget(Newtarget);
}*/

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

/*void APlayerCharacter::SwitchTargetRight(const FInputActionValue& Value) // t
{
    if (!CanAct(EActionType::LockOn) || !IsValid(LockOnTarget))
        return;

    AEnemyBase* NewTarget = SwitchTarget(false);
    if (!IsValid(NewTarget) || NewTarget == LockOnTarget)
        return;

    ClearLockOn();
    SetLockOnTarget(NewTarget);
}*/

/*void APlayerCharacter::CheckLockOnDistance() // t
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
    
    //if (LockOnTarget->IsDead())
    //{
    //    ClearLockOn();
    //}
    // 위 코드를 추가할 시 따로 if문을 만들지 말고 or 연산으로 위 if문에 추가할 것.
}*/

// 아마 안쓸듯.
FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
    return FGenericTeamId(TeamID);
}

/*void APlayerCharacter::ClearLockOn() // t
{
    if (!IsValid(LockOnTarget))
        return;

    LockOnTarget->ShowTargetWidget(false);
    LockOnTarget->ResetHPBarTimer();
    LockOnTarget->SetIsTargeted(false);
    LockOnTarget = nullptr;

    // 이동 모드 복구: 가고자 하는 방향으로 몸을 돌림
    SetLockOnState(false);
}

// 타겟팅 컴포넌트로 옮길 것.
// 앞에 owner-> 이런 형식으로 컴포넌트에서 플레이어를 조작.
void APlayerCharacter::SetLockOnState(bool bIsLockOn) // p
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
}*/

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

// 컴포넌트의 함수를 호출하는 래핑 함수. 건드릴 필요 없음.
void APlayerCharacter::ToggleInventory(const FInputActionValue& Value)
{
    // 전부 UI가 있는 컨트롤러로 옮겨졌음.
    if (!CachedController)
        return;

    CachedController->ToggleInventory();
}

void APlayerCharacter::HealPlayer(float PlusHP) // s
{
    UE_LOG(LogTemp, Warning, TEXT("PrevHP : %f"), CurHP);
    CurHP = FMath::Min(CurHP + PlusHP, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), CurHP);
}

/*void APlayerCharacter::SetLockOnTarget(AEnemyBase* NewTarget) // t 
{
    if (!IsValid(NewTarget))
        return;

    LockOnTarget = NewTarget;
    LockOnTarget->ShowTargetWidget(true);
    LockOnTarget->ShowHPBarWidget(true);
    LockOnTarget->SetIsTargeted(true);
    SetLockOnState(true); 
}*/

void APlayerCharacter::SetEquippedWeapon(AWeaponBase* CurWeapon)
{
    EquippedWeapon = CurWeapon;
}

void APlayerCharacter::SetWeaponVisible(bool IsVisible)
{
    if (!IsValid(EquippedWeapon))
        return;

    if (IsVisible)
    {
        EquippedWeapon->SetActorHiddenInGame(true);
    }
    else
    {
        EquippedWeapon->SetActorHiddenInGame(false);
    }
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (CurHP <= 0.f || CurrentState == EPlayerState::Dodging)
        return ActualDamage;

    UAnimInstance* Anim = GetMesh()->GetAnimInstance();
    if (!Anim)
        return ActualDamage;

    CurHP = FMath::Clamp(CurHP - ActualDamage, 0.f, MaxHP);
    CachedController->SetHPPercent(CurHP / MaxHP);

    if (CurHP <= 0.f)
    {
        // 사망 애니메이션 및 후속 처리.
        if (DieMontages.Num() == 0)
            return ActualDamage;

        int32 DieIndex = FMath::RandRange(0, DamagedMontages.Num() - 1);
        Anim->Montage_Play(DieMontages[DieIndex]);
    }
    else
    {
        // 현재 체력이 ActualDamage만큼 줄어든다.
        // 만약 체력이 0보다 작다면 사망.
        // 피격 애니메이션 실행.
        CurrentState = EPlayerState::Damaged;
        if (DamagedMontages.Num() == 0)
            return ActualDamage;

        UE_LOG(LogTemp, Warning, TEXT("Player Take %f Damage!"), ActualDamage);

        //
        int32 DamagedIndex = FMath::RandRange(0, DamagedMontages.Num() - 1);
        Anim->Montage_Play(DamagedMontages[DamagedIndex]);
    }

    return ActualDamage;
}

bool APlayerCharacter::HasLockTarget() const // p
{
    //return LockOnTarget == nullptr ? false : true;

    // PlayerBaseComponent의 래핑함수.
    return TargetCom->HasLockTarget();
}

void APlayerCharacter::SetCanNextComboTrue() // a
{
    //bCanNextCombo = true;

    // AttackComponent의 래핑함수.
    AttackCom->SetCanNextComboTrue();
}

void APlayerCharacter::ConsumeStamina(float Cost)
{
    CurStamina = FMath::Clamp(CurStamina - Cost, 0.f, MaxStamina);

    CachedController->SetStaminaPercent(CurStamina / MaxStamina);
}

bool APlayerCharacter::CanPlusStamina() const
{
    if (CurStamina >= MaxStamina)
        return false;

    switch (CurrentState)
    {
    case EPlayerState::Idle:
    case EPlayerState::Moving:
    case EPlayerState::Damaged:
        return true;
    case EPlayerState::Attacking:
    case EPlayerState::Dodging:
    case EPlayerState::Dead:
        return false;
    }
    return false;
}

void APlayerCharacter::OnActStart()
{
    ConsumeStamina(StaminaCost);
}

void APlayerCharacter::UseSkill(const FInputActionInstance& Instance)
{
    FName SkillID = Instance.GetSourceAction()->GetFName();
    UE_LOG(LogTemp, Warning, TEXT("%s"), *SkillID.ToString());

    SkillBaseCom->UseSkill(SkillID);
}