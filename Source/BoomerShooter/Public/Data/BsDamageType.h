#pragma once

#include "BsDamageType.generated.h"

UCLASS()
class UBsDamageType : public UDamageType
{
	GENERATED_BODY()	
};

UCLASS()
class UBsBleedDamageType : public UBsDamageType
{
	GENERATED_BODY()

protected:

	float Duration = 2.0f;
	float Interval = 0.5f;
	float Damage = 10.f;

public:
	
	FORCEINLINE float GetBleedDuration() const { return Duration; }
	FORCEINLINE float GetBleedInterval() const { return Interval; }
	FORCEINLINE float GetBleedDamage() const { return Damage; }
};

UCLASS()
class UBsSkeletonMeleeDamageType : public UBsDamageType
{
	GENERATED_BODY()
};

UCLASS()
class UBsExplosionDamageType : public UBsDamageType
{
	GENERATED_BODY()	
};