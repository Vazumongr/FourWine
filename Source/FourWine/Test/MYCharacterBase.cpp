// Troy Records Jr. 2021

#include "MYCharacterBase.h"

#include "Net/UnrealNetwork.h"

AMYCharacterBase::AMYCharacterBase()
{
	
}
void AMYCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMYCharacterBase, RightHandWeapon);
	DOREPLIFETIME(AMYCharacterBase, LeftHandWeapon);
	DOREPLIFETIME(AMYCharacterBase, WeaponClass);
	DOREPLIFETIME(AMYCharacterBase, WeaponItemThing);
}