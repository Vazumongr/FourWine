#include "FWAbilitySystemComponent.h"

void UFWAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	UE_LOG(LogTemp, Warning, TEXT("%s - %i"), *FString(__FUNCTION__), InputID);
	Super::AbilityLocalInputPressed(InputID);
}
