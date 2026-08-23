#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBossHPBar.generated.h"

UCLASS()
class ROGUEHEART_API UEnemyBossHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHPPercent(float Percent);
};
