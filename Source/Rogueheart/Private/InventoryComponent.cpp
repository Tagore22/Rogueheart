#include "InventoryComponent.h"
#include "Engine/DataTable.h"
#include "RogueheartGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Character/Player/PlayerCharacter.h"

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
        UE_LOG(LogTemp, Warning, TEXT("Item Stacked: %s (New Count: %d)"), *ItemID.ToString(), *CurrentCount);
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

    UE_LOG(LogTemp, Warning, TEXT("New Item Added: %s (Count: %d)"), *ItemID.ToString(), Count);
    return true;
}

/*bool UInventoryComponent::UseItem(FName ItemID)
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
    UE_LOG(LogTemp, Warning, TEXT("Item Use: %s"), *ItemID.ToString());
    return true;
}*/

bool UInventoryComponent::UseItem(FName ItemID)
{
    // 1. 인벤토리 내 존재 확인
    int32* CurrentCount = ItemCounts.Find(ItemID);
    if (!CurrentCount || *CurrentCount <= 0)
    {
        return false;
    }
    // 2. 아이템 데이터 테이블에서 정보 가져오기 
    // (사용자의 데이터 매니저나 구조에 맞게 수정하세요)
    // 현재 인벤토리가 게임인스턴스에서 생성된다. GetOwner()는 액터에 붙어있는 컴포넌트들만 가지기에
    // 아래에서 GetOuter() 대신 GetOwner()를 호출하면 nullptr이 반환된다.
    URogueheartGameInstance* GI = Cast<URogueheartGameInstance>(GetOuter());
    if (!GI)
        return false;
    UDataTable* ItemTable = GI ? GI->GetItemDataTable() : nullptr;
    if (!ItemTable)
        return false;
    const FItemData* ItemData = ItemTable->FindRow<FItemData>(ItemID, TEXT(""));
    if (!ItemData)
        return false;

    // 3. 아이템 타입에 따른 분기 처리
    switch (ItemData->ItemType)
    {
    case EItemType::Equipment:
        // [장착 로직] 수량은 줄이지 않고 장착 상태만 변경 (Toggle 방식)
        if (EquippedWeaponID == ItemID)
        {
            EquippedWeaponID = NAME_None; // 이미 장착 중이면 해제
            UE_LOG(LogTemp, Warning, TEXT("Item Unequipped: %s"), *ItemID.ToString());
        }
        else
        {
            EquippedWeaponID = ItemID;    // 새로 장착
            UE_LOG(LogTemp, Warning, TEXT("Item Equipped: %s"), *ItemID.ToString());
        }
        break;

    case EItemType::Consumable:
        // [소모 로직] 수량 감소 및 효과 적용
        *CurrentCount -= 1;
        UE_LOG(LogTemp, Warning, TEXT("Item Consumed: %s (Remaining: %d)"), *ItemID.ToString(), *CurrentCount);

        // 효과 적용 로직 (예: HealPlayer(ItemData->EffectValue))을 여기에 추가
        // 2. 게임 인스턴스를 통해 컨트롤러 획득
        if (APlayerController* PC = GI->GetFirstLocalPlayerController())
        {
            if (APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetCharacter()))
            {
                Player->HealPlayer(ItemData->EffectValue);
            }
        }
        // 수량이 다 떨어졌을 때 처리
        if (*CurrentCount <= 0)
        {
            ItemCounts.Remove(ItemID);
            ItemIDs.Remove(ItemID);
            UE_LOG(LogTemp, Warning, TEXT("Item Delete"));
            // 혹시라도 장착 중인 소모품(화살 등)이었다면 해제
            if (EquippedWeaponID == ItemID)
            {
                EquippedWeaponID = NAME_None;
            }
        }
        break;

    case EItemType::Material:
    case EItemType::KeyItem:
        // 재료나 퀘스트 아이템은 '사용' 개념이 다를 수 있으므로 일단 보류
        UE_LOG(LogTemp, Warning, TEXT("This item cannot be used directly: %s"), *ItemID.ToString());
        return false;

    default:
        break;
    }
    // 4. 데이터가 변했으므로 UI 갱신을 위한 델리게이트 호출 등을 여기에 배치
    // OnInventoryUpdated.Broadcast();
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
    UE_LOG(LogTemp, Warning, TEXT("Equip success : %s"), *ItemID.ToString());
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

