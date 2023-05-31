#include "AnimInstance/BsScytheAnimInstance.h"
#include "Weapon/Scythe/BsScythe.h"

void UBsScytheAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (ABsScythe* Owner = Cast<ABsScythe>(GetOwningActor()))
	{
		Scythe = Owner;
	}
}

void UBsScytheAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	Scythe = Scythe ? Scythe : Cast<ABsScythe>(GetOwningActor());
	if (!Scythe)
	{
		return;
	}

	bMeleeMode = Scythe->IsMeleeMode();
	bRangedMode = Scythe->IsRangedMode();
	bThrown = Scythe->IsThrown();
	bAttachedToGrapplePoint = Scythe->IsAttachedToGrapplePoint();
}
