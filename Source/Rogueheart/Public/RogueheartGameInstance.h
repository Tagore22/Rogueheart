#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RogueheartGameInstance.generated.h"

class UUIManager;

UCLASS()
class ROGUEHEART_API URogueheartGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UUIManager* GetUIManager() const { return UIManager; }

private:
    UPROPERTY()
    UUIManager* UIManager;
};