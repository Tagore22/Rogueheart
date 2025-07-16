#include "RogueheartGameInstance.h"
#include "UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
//
#include "Engine/Engine.h"

void URogueheartGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Warning, TEXT(">>> RogueheartGameInstance::Init() called"));

    UIManager = NewObject<UUIManager>(this, UUIManager::StaticClass());
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT(">>> Got PlayerController, initialize UIManager"));
        UIManager->Initialize(PC);
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT(">>> RogueheartGameInstance::Init() called"));
    }
}