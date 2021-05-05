// Troy Records Jr. 2021


#include "FWInventoryItemWidget.h"
#include "FourWine/Characters/FWPlayerCharacter.h"

void UFWInventoryItemWidget::EquipWeapon()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if(PlayerController!= nullptr)
	{
		AFWPlayerCharacter* PlayerCharacter = Cast<AFWPlayerCharacter>(PlayerController->GetPawn());
		PlayerCharacter->EquipWeapon(Idx);
	}
}
