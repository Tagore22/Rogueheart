#include "Character/Player/RogueheartPlayerController.h"
#include "UI/UIManager.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UI/PlayerHPBarWidget.h"
#include "StatSubsystem.h"
#include "Character/Player/PlayerCharacter.h"

ARogueheartPlayerController::ARogueheartPlayerController()
{
    // BP_UIManager 경로: Content Browser 상 폴더 구조에 맞춰서
    static ConstructorHelpers::FClassFinder<UUIManager> UIManagerBPClass(
        TEXT("/Game/Characters/BP_UIManager.BP_UIManager_C")
    ); 
    if (UIManagerBPClass.Succeeded())
        UIManagerClass = UIManagerBPClass.Class;
}

void ARogueheartPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UIManagerClass && !IsValid(UIManager))
    {
        // PlayerController를 Outer로 해서 생성
        UIManager = NewObject<UUIManager>(this, UIManagerClass);
        UIManager->Initialize(this);
    }
    if (WBP_Inventory)
    {
        InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_Inventory);
    }
    if (WBP_HPBar)
    {
        HPBarWidget = CreateWidget<UPlayerHPBarWidget>(GetWorld(), WBP_HPBar);
        HPBarWidget->AddToViewport(); 
    }
    if (WBP_LevelUp)
    {
        LevelUpWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_LevelUp);
    }
    InitializeStat();
}

void ARogueheartPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void ARogueheartPlayerController::ToggleInventory()
{
    // 인벤토리가 켜져있으면 끄고, 꺼져있으면 킨다.
    // 아마 현재 UI가 nullptr인지로 알 수 있다.
    UE_LOG(LogTemp, Warning, TEXT("Inventory Toggle"));
    //UE_LOG(LogTemp, Log, TEXT("LockOnTarget: nullptr"));

    if (!InventoryWidget)
        return;

    bool bIsActivate = InventoryWidget->IsInViewport();
     // 현재 인벤토리가 켜져 있음.
    if (bIsActivate)
    {
        InventoryWidget->RemoveFromParent();
    }
    else
    {
        InventoryWidget->AddToViewport();
    }
    ActivateUI(bIsActivate);
}

void ARogueheartPlayerController::ToggleLevelUp()
{
    // 인벤토리가 켜져있으면 끄고, 꺼져있으면 킨다.
    // 아마 현재 UI가 nullptr인지로 알 수 있다.
    UE_LOG(LogTemp, Warning, TEXT("LevelUp Toggle"));

    if (!LevelUpWidget)
        return;

    bool bIsActivate = LevelUpWidget->IsInViewport();
    // 현재 인벤토리가 켜져 있음.
    if (bIsActivate)
    {
        LevelUpWidget->RemoveFromParent();
    }
    else
    {
        LevelUpWidget->AddToViewport();
    }
    ActivateUI(bIsActivate);
}

void ARogueheartPlayerController::SetHPPercent(float Percent)
{
    if (!HPBarWidget)
    {
        return;
    }

    HPBarWidget->SetHPPercent(Percent);
}

void ARogueheartPlayerController::SetMPPercent(float Percent)
{
    if (!HPBarWidget)
    {
        return;
    }

    HPBarWidget->SetMPPercent(Percent);
}

void ARogueheartPlayerController::SetStaminaPercent(float Percent)
{
    if (!HPBarWidget)
    {
        return;
    }

    HPBarWidget->SetStaminaPercent(Percent);
}

void ARogueheartPlayerController::HPWidthOverride(float Width)
{
    if (!HPBarWidget)
    {
        return;
    }

    HPBarWidget->HPWidthOverride(Width);
}

void ARogueheartPlayerController::MPWidthOverride(float Width)
{
    if (!HPBarWidget)
    {
        return;
    }

    HPBarWidget->MPWidthOverride(Width);
}

void ARogueheartPlayerController::StaminaWidthOverride(float Width)
{
    if (!HPBarWidget)
    {
        return;
    }

    HPBarWidget->StaminaWidthOverride(Width);
}

void ARogueheartPlayerController::SetTextBlock(int32 Num)
{
    if (!HPBarWidget)
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Nullptr!"));
    HPBarWidget->SetTextBlock(Num);
}

void ARogueheartPlayerController::InitializeStat()
{
    APlayerCharacter* PlayerAct = Cast<APlayerCharacter>(GetCharacter());
    UStatSubsystem* StatData = GetGameInstance()->GetSubsystem<UStatSubsystem>();
    if (!IsValid(PlayerAct) || !StatData)
    {
        return;
    }

    float MaxHP = StatData->GetMaxHP();
    float CurHP = StatData->GetCurHP();
    PlayerAct->SetMaxHP(MaxHP);
    PlayerAct->SetCurHP(CurHP);
    SetHPPercent(CurHP / MaxHP);

    float MaxMP = StatData->GetMaxMP();
    float CurMP = StatData->GetCurMP();
    PlayerAct->SetMaxMana(MaxMP);
    PlayerAct->SetCurMana(CurMP);
    SetMPPercent(CurMP / MaxMP);

    float MaxStamina = StatData->GetMaxStamina();
    float CurStamina = StatData->GetCurStamina();
    PlayerAct->SetMaxStamina(MaxStamina);
    PlayerAct->SetCurStamina(CurStamina);
    SetStaminaPercent(CurStamina / MaxStamina);

    PlayerAct->SetSoulSum(StatData->GetSoulSum());
    SetTextBlock(PlayerAct->GetSoulSum());

    HPBarWidget->UIWidthInitialize(MaxHP, MaxMP, MaxStamina);
}

void ARogueheartPlayerController::ActivateUI(bool bIsActivate)
{
    if (bIsActivate)
    {
        // 마우스 커서 숨기고 게임 입력으로 전환
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        SetShowMouseCursor(false);
        SetPause(false);
    }
    else
    {
        // 마우스 커서 보이고 UI 입력으로 전환
        // 인벤토리를 끌 때에도 키보드를 입력하는 Game모드이기에 UIOnly는 불가능하다.
        FInputModeGameAndUI InputMode;
        // 마우스 클릭 시 커서가 갑자기 사라지는 걸 방지
        InputMode.SetHideCursorDuringCapture(false);
        InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
        SetInputMode(InputMode);
        SetShowMouseCursor(true);
        SetPause(true);
    }
}
