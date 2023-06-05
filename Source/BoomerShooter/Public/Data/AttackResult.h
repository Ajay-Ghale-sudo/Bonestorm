#pragma once

UENUM(BlueprintType)
enum class EAttackResult : uint8
{
	EAR_None		UMETA(DisplayName = "None"),
	EAR_NoLOS		UMETA(DisplayName = "No Line of Sight"),
	EAR_TooClose	UMETA(DisplayName = "Too Close"),
	EAR_TooFar		UMETA(DisplayName = "Too Far"),
    EAR_Successful	UMETA(DisplayName = "Successful"),
	
	EAR_MAX UMETA(DisplayName = "MAX")
};