#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelupWidget.generated.h"

UCLASS()
class ROGUEHEART_API ULevelupWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnHPUpClicked();

	UFUNCTION()
	void OnMPUpClicked();

	UFUNCTION()
	void OnStaminaUpClicked();

	UFUNCTION()
	void OnSkillQUpClicked();

	UFUNCTION()
	void OnSkillEUpClicked();

	UFUNCTION()
	void OnSkillRUpClicked();
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* HPUpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MPUpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* StaminaUpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillQUpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillEUpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillRUpButton;

	UPROPERTY(EditDefaultsOnly)
	float PlusValue = 10.f;

	UPROPERTY()
	class APlayerCharacter* Player;
};
