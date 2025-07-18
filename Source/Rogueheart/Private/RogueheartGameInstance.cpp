#include "RogueheartGameInstance.h"
#include "UI/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

void URogueheartGameInstance::Init()
{
    Super::Init();

    UIManager = NewObject<UUIManager>(this, UUIManager::StaticClass());
}