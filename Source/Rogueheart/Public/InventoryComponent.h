#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUEHEART_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    // 아이템 추가 (TMap에 개수 증가, TArray에 순서 관리)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(FName ItemID, int32 Count = 1);

    // 아이템 사용 (TMap 개수 감소, 0 이하 시 TArray에서도 제거)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UseItem(FName ItemID);

    // 장비 장착 (인벤토리에 있는지 확인 후 장착)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool EquipItem(FName ItemID);

    // 현재 장착 중인 무기 ID
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    FName EquippedWeaponID = NAME_None;

    // UI 표시용 정렬된 키 목록
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    const TArray<FName>& GetItemIDs() const { return ItemIDs; }

    // 최대 슬롯 수
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
    int32 MaxSlots = 20;

protected:
    // 개수 관리 (빠른 조회용)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TMap<FName, int32> ItemCounts;

    // 순서 관리 (UI 표시용, 삽입 순서 유지)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FName> ItemIDs;
};