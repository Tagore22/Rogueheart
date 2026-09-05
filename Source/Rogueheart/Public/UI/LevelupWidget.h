#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelupWidget.generated.h"

UCLASS()
class ROGUEHEART_API ULevelupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTextHPLevel(int32 Num);

	void SetTextMPLevel(int32 Num);

	void SetTextStaminaLevel(int32 Num);

	void SetTextSkillQLevel(int32 Num);

	void SetTextSkillELevel(int32 Num);

	void SetTextSkillRLevel(int32 Num);

protected:
	virtual void NativeOnInitialized() override;

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

	void SkillLevelUp(FName SkillName);
	
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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPLevel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MPLevel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StaminaLevel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillQLevel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillELevel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillRLevel;
};
