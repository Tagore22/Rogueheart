#include "Character/Player/PlayerBaseComponent.h"

UPlayerBaseComponent::UPlayerBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerBaseComponent::SetLockOnState(bool bIsLockOn)
{
    if (bIsLockOn)
    {
        Player->GetCharacterMovement()->bOrientRotationToMovement = false;
        Player->bUseControllerRotationYaw = true;
    }
    else
    {
        Player->GetCharacterMovement()->bOrientRotationToMovement = true;
        Player->bUseControllerRotationYaw = false;
    }
}

void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerBaseComponent::SetupInputBinding(UEnhancedInputComponent* EnhancedInput)
{

}

