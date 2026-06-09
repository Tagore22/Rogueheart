#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

UCLASS()
class ROGUEHEART_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHPPercent(const float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;
};
