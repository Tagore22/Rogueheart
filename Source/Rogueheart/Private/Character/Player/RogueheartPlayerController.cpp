#include "Character/Player/RogueheartPlayerController.h"
#include "UI/UIManager.h"
#include "UObject/ConstructorHelpers.h"

ARogueheartPlayerController::ARogueheartPlayerController()
{
    // BP_UIManager ���: Content Browser �� ���� ������ ���缭
    static ConstructorHelpers::FClassFinder<UUIManager> UIManagerBPClass(
        TEXT("/Game/Characters/BP_UIManager.BP_UIManager_C")
    ); 
    if (UIManagerBPClass.Succeeded())
        UIManagerClass = UIManagerBPClass.Class;
}

void ARogueheartPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UIManagerClass && !UIManager)
    {
        // PlayerController�� Outer�� �ؼ� ����
        UIManager = NewObject<UUIManager>(this, UIManagerClass);
        UIManager->Initialize(this);
    }
}