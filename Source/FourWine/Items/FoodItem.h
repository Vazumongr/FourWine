// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"
#include "FourWine/Items/Item.h"
#include "FoodItem.generated.h"

/**
 * 
 */
UCLASS()
class FOURWINE_API UFoodItem : public UItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	float HealthToHeal = 20.f;
	
protected:

	virtual void Use(class AFourWineCharacter* Character) override;
	
};
