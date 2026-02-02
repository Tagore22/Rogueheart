#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InventoryComponent.h"
#include "RogueheartGameInstance.generated.h"

class UUIManager;
class UDataTable;

UCLASS(Blueprintable, BlueprintType)
class ROGUEHEART_API URogueheartGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UUIManager* GetUIManager() const { return UIManager; }
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
    UDataTable* GetItemDataTable() const { return ItemDataTable; }
private:
    UPROPERTY()
    UUIManager* UIManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    UDataTable* ItemDataTable;
};