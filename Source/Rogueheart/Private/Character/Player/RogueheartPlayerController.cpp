#include "Character/Player/RogueheartPlayerController.h"
#include "UI/UIManager.h"
#include "UObject/ConstructorHelpers.h"

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

    if (UIManagerClass && !UIManager)
    {
        // PlayerController를 Outer로 해서 생성
        UIManager = NewObject<UUIManager>(this, UIManagerClass);
        UIManager->Initialize(this);
    }
}