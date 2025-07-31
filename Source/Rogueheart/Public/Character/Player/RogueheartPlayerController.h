#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RogueheartPlayerController.generated.h"

class UUIManager;

/** �÷��̾� ��Ʈ�ѷ����� UIManager ���� */
UCLASS()
class ROGUEHEART_API ARogueheartPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ARogueheartPlayerController();

    virtual void BeginPlay() override;

private:
    /** BP_UIManager ����Ŭ������ ���⿡ ���� */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUIManager> UIManagerClass;

    /** ��Ÿ�ӿ� ������ �Ŵ��� �ν��Ͻ� */
    UPROPERTY()
    UUIManager* UIManager;
};