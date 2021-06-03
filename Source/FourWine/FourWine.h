// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAttribute, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWeapon, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogInventory, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPlayer, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWisp, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogLoot, All, All);

UENUM(BlueprintType)
enum class EFWAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	Ability1		UMETA(DisplayName = "Ability1"),
	// 4 RMB
	Ability2		UMETA(DisplayName = "Ability2"),
	// 8 Sprint
	Sprint			UMETA(DisplayName = "Sprint"),
	// 9 Jump
	Jump			UMETA(DisplayName = "Jump")
};