#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

UCLASS()
class ROGUEHEART_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHPPercent(float Percent);

	void SetDamageSum(float Damage);

	void ResetDamageSum();
protected:
	//virtual void NativeConstruct() override;
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;

	float DamageSum = 0.f;
};
