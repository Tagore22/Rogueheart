#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/RogueheartPlayerController.h"
#include "Rogueheart.h" 
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"
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

	virtual void SetupInputBinding(class UEnhancedInputComponent* EnhancedInput); // 자체 함수.
protected:
	UPROPERTY()
	class APlayerCharacter* Player = nullptr; //
};
