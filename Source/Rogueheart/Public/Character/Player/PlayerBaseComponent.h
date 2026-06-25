#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "PlayerBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUEHEART_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerBaseComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* EnhancedInput);

protected:
	UPROPERTY()
	class APlayerCharacter* Player = nullptr; 

	UPROPERTY()
	class AEnemyBase* LockOnTarget = nullptr;
};
