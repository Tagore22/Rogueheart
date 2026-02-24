#include "Character/Player/RogueheartPlayerController.h"
#include "UI/UIManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

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
        HPBarWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_HPBar);
        HPBarWidget->AddToViewport(); 
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

     // 현재 인벤토리가 켜져 있음.
    if (InventoryWidget->IsInViewport())
    {
        InventoryWidget->RemoveFromParent();

        // 마우스 커서 숨기고 게임 입력으로 전환
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        bShowMouseCursor = false;
        SetPause(false);
    }
    else
    {
        InventoryWidget->AddToViewport();

        // 마우스 커서 보이고 UI 입력으로 전환
        FInputModeGameAndUI InputMode;
        // 마우스 클릭 시 커서가 갑자기 사라지는 걸 방지
        InputMode.SetHideCursorDuringCapture(false);
        InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
        SetInputMode(InputMode);
        bShowMouseCursor = true;
        SetPause(true);
    }
}