#include "Character/Player/TargetComponent.h"

void UTargetComponent::SetupInputBinding(UEnhancedInputComponent* EnhancedInput)
{
    Super::SetupInputBinding(EnhancedInput);

    EnhancedInput->BindAction(IA_LockOn, ETriggerEvent::Started, this, &UTargetComponent::ToggleLockOn);
    EnhancedInput->BindAction(IA_SwitchTargetLeft, ETriggerEvent::Started, this, &UTargetComponent::SwitchTargetLeft);
    EnhancedInput->BindAction(IA_SwitchTargetRight, ETriggerEvent::Started, this, &UTargetComponent::SwitchTargetRight);
}

void UTargetComponent::UpdateLockOnRotation(float DeltaTime)
{
    if (!IsValid(LockOnTarget))
        return;

    FVector CameraDir = LockOnTarget->GetActorLocation() - Player->GetActorLocation();
    if (CameraDir.IsNearlyZero())
        return;

    FVector TargetDir = FVector(CameraDir.X, CameraDir.Y, 0.f);
    FRotator NewActorRot = FMath::RInterpTo(Player->GetActorRotation(), TargetDir.Rotation(), DeltaTime, InterpSpeed);
    Player->SetActorRotation(NewActorRot);

    if (!CachedController)
        return;

    FRotator NewCameraRot = FMath::RInterpTo(Player->GetControlRotation(), CameraDir.Rotation(), DeltaTime, InterpSpeed);
    CachedController->SetControlRotation(NewCameraRot);
}

// 타겟팅 컴포넌트로 옮길 것.
void UTargetComponent::ClearLockOn()
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

AEnemyBase* UTargetComponent::FindNearestTarget()
{
    // 1단계: 주변 적들 긁어모으기 (Wide Overlap)
    FVector CameraLocation = Player->GetCameraComponent()->GetComponentLocation();
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Player); // 나 자신 제외

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
    FVector CameraForward = Player->GetCameraComponent()->GetForwardVector();

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
        TraceParams.AddIgnoredActor(Player);

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
}

AEnemyBase* UTargetComponent::SwitchTarget(bool bLeft)
{
    if (!IsValid(LockOnTarget))
        return nullptr;

    // 1. 범위 내 적들만 필터링, OverlapMultiByObjectType 혹은 OverlapMultiByChannel
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Player); // 나 자신 제외

    // 구체(Sphere) 범위를 생성하여 해당 채널(예: ECC_Pawn)의 물체를 탐색
    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        Player->GetActorLocation(),
        FQuat::Identity,
        TraceChannel::ECC_Player, // 적의 콜리전 채널에 맞게 변경 가능. 다만 충돌 반응을 반드시 block으로 할 것.
                             // OnComponentBeginOverlap()과 같이 무조건 overlap이 아닌 경우에는 block을 쓸 것.
        FCollisionShape::MakeSphere(LockOnRange),
        QueryParams
    );

    if (!bHit)
        return nullptr;

    FVector MyLocation = Player->GetActorLocation();
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

void UTargetComponent::ToggleLockOn(const FInputActionValue& Value)
{
    if (!Player->CanAct(EActionType::LockOn))
        return;

    if (IsValid(LockOnTarget))
    {
        ClearLockOn();
        //PrevLockOnTarget = nullptr;
    }
    else
    {
        AEnemyBase* NewTarget = FindNearestTarget();
        if (IsValid(NewTarget))
        {
            SetLockOnTarget(NewTarget);
        }
    }
}

void UTargetComponent::SwitchTargetLeft(const FInputActionValue& Value)
{
    if (!Player->CanAct(EActionType::LockOn) || !IsValid(LockOnTarget))
        return;

    AEnemyBase* Newtarget = SwitchTarget(true);
    if (!IsValid(Newtarget) || Newtarget == LockOnTarget)
        return;

    ClearLockOn();
    SetLockOnTarget(Newtarget);
}

void UTargetComponent::SwitchTargetRight(const FInputActionValue& Value)
{
    if (!Player->CanAct(EActionType::LockOn) || !IsValid(LockOnTarget))
        return;

    AEnemyBase* NewTarget = SwitchTarget(false);
    if (!IsValid(NewTarget) || NewTarget == LockOnTarget)
        return;

    ClearLockOn();
    SetLockOnTarget(NewTarget);
}

void UTargetComponent::SetLockOnTarget(AEnemyBase* NewTarget)
{
    if (!IsValid(NewTarget))
        return;

    LockOnTarget = NewTarget;
    LockOnTarget->ShowTargetWidget(true);
    LockOnTarget->ShowHPBarWidget(true);
    LockOnTarget->SetIsTargeted(true);
    SetLockOnState(true);
}
