// Troy Records Jr. 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class FOURWINE_API UQuestTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* QuestTitle;
	
};
