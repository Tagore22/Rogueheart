#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItem(FName ItemID, int32 Count)
{
    if (ItemID.IsNone() || Count <= 0)
        return false;

    // 1. 이미 존재하는 아이템인지 확인 (포인터 방식 사용)
    if (int32* CurrentCount = ItemCounts.Find(ItemID))
    {
        *CurrentCount += Count;
        UE_LOG(LogTemp, Log, TEXT("Item Stacked: %s (New Count: %d)"), *ItemID.ToString(), *CurrentCount);
        return true;
    }

    // 2. 새로운 아이템인 경우 슬롯 부족 여부 확인
    if (ItemCounts.Num() >= MaxSlots)
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory Full! Cannot add: %s"), *ItemID.ToString());
        return false;
    }

    // 3. 데이터 추가 (Map과 Array 동시 업데이트)
    ItemCounts.Add(ItemID, Count);
    ItemIDs.Add(ItemID);

    UE_LOG(LogTemp, Log, TEXT("New Item Added: %s (Count: %d)"), *ItemID.ToString(), Count);
    return true;
}

bool UInventoryComponent::UseItem(FName ItemID)
{
    int32* CurrentCount = ItemCounts.Find(ItemID);
    if (!CurrentCount)
        return false;

    // 사용 로직 (나중에 EffectType 보고 실제 효과 적용)
    *CurrentCount -= 1;

    if (*CurrentCount <= 0)
    {
        ItemCounts.Remove(ItemID);
        ItemIDs.Remove(ItemID);

        // [추가] 장착 중인 아이템이었다면 장착 해제
        if (EquippedWeaponID == ItemID)
        {
            EquippedWeaponID = NAME_None;
            UE_LOG(LogTemp, Warning, TEXT("Equipped item consumed and unequipped: %s"), *ItemID.ToString());
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Item Use: %s"), *ItemID.ToString());
    return true;
}

bool UInventoryComponent::EquipItem(FName ItemID)
{
    int32* Current = ItemCounts.Find(ItemID);
    if (!Current)
    {
        UE_LOG(LogTemp, Warning, TEXT("Equip fault - don't has item : %s"), *ItemID.ToString());
        return false;
    }

    EquippedWeaponID = ItemID;
    UE_LOG(LogTemp, Log, TEXT("Equip success : %s"), *ItemID.ToString());
    return true;
}

/*void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}*/

/*void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}*/

