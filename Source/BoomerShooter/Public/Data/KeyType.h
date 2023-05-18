#pragma once


UENUM(BlueprintType)
enum class EBsKeyType : uint8
{
	EKT_None	  UMETA(DisplayName = "None"),
	EKT_BronzeKey UMETA(DisplayName = "Bronze Key"),
	EKT_SilverKey UMETA(DisplayName = "Silver Key"),
	EKT_GoldenKey UMETA(DisplayName = "Golden Key"),

	EKT_MAX UMETA(DisplayName = "DefaultMAX")
};