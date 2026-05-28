#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RogueheartPlayerController.generated.h"

/** 플레이어 컨트롤러에서 UIManager 생성 */
UCLASS()
class ROGUEHEART_API ARogueheartPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ARogueheartPlayerController();

    void ToggleInventory();
protected:
    /** BP_UIManager 서브클래스를 여기에 저장 */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUIManager> UIManagerClass;

    /** 런타임에 생성될 매니저 인스턴스 */
    UPROPERTY()
    class UUIManager* UIManager;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> WBP_Inventory;

    UPROPERTY()
    class UUserWidget* InventoryWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> WBP_HPBar;

    UPROPERTY()
    class UUserWidget* HPBarWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;
};