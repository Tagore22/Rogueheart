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

void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerBaseComponent::SetupInputBinding(UEnhancedInputComponent* EnhancedInput)
{

}

