// Troy Records Jr. 2021


#include "FoodItem.h"

#include "FourWine/FourWineCharacter.h"

void UFoodItem::Use(AFourWineCharacter* Character)
{
    Character->Heal(HealthToHeal);
}
