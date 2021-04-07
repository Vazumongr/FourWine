// Copyright 2021 Troy Records Jr.

#include "FWGameplayAbility.h"
#include "AbilitySystemComponent.h"

UFWGameplayAbility::UFWGameplayAbility()
{
	// Default to Instance Per Actor TODO figure out wtf this means
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFWGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
