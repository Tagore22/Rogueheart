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

    int32 GetMaxHP() const { return MaxHP; }
    int32 GetCurHP() const { return CurHP; }
    int32 GetMaxStamina() const { return MaxStamina; }
    int32 GetCurStamina() const { return CurStamina; }

    void SetMaxHP(int NewMaxHP) { MaxHP = NewMaxHP; }
    void SetCurHP(int NewCurHP) { CurHP = NewCurHP; }
    void SetMaxStamina(int NewMaxStamina) { MaxStamina = NewMaxStamina; }
    void SetCurStamina(int NewCurStamina) { CurHP = NewCurStamina; }

private:
    UPROPERTY()
    UUIManager* UIManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Data")
    UDataTable* ItemDataTable;

    int32 MaxHP = 100;
    int32 CurHP = 100;
    int32 MaxStamina = 100;
    int32 CurStamina = 100;
};