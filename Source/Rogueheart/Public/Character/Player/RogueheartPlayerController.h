#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RogueheartPlayerController.generated.h"

class UUIManager;

/** 플레이어 컨트롤러에서 UIManager 생성 */
UCLASS()
class ROGUEHEART_API ARogueheartPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ARogueheartPlayerController();

    virtual void BeginPlay() override;

private:
    /** BP_UIManager 서브클래스를 여기에 저장 */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUIManager> UIManagerClass;

    /** 런타임에 생성될 매니저 인스턴스 */
    UPROPERTY()
    UUIManager* UIManager;
};