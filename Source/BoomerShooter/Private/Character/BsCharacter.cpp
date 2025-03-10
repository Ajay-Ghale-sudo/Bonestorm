﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BsCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/BsGrappleHookComponent.h"
#include "Component/BsHealthComponent.h"
#include "Component/BsInventoryComponent.h"
#include "Component/Audio/BsCharacterAudioComponent.h"
#include "Component/Camera/BsCameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Props/Head/BsSeveredHeadBase.h"
#include "Weapon/BsWeaponBase.h"
#include "Weapon/Scythe/BsScythe.h"

// Sets default values
ABsCharacter::ABsCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	CameraArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArmComponent->SetupAttachment(GetCapsuleComponent());
	CameraArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	
	CameraComponent = CreateDefaultSubobject<UBsCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraArmComponent);

	WeaponSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("WeaponSpringArm"));
	WeaponSpringArmComponent->SetupAttachment(CameraComponent);
	WeaponSpringArmComponent->bDoCollisionTest = false;
	WeaponSpringArmComponent->TargetArmLength = 60.f;

	InventoryComponent = CreateDefaultSubobject<UBsInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UBsHealthComponent>(TEXT("HealthComponent"));
	AudioComponent = CreateDefaultSubobject<UBsCharacterAudioComponent>(TEXT("AudioComponent"));
}

// Called when the game starts or when spawned
void ABsCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputConfig.DefaultInputContext, 0);
		}
	}

	if (const UCapsuleComponent* Capsule = GetCapsuleComponent())
	{

		SlideConfig.PreSlideHeight = Capsule->GetUnscaledCapsuleHalfHeight();
	}
	
	JumpMaxCount = 2;
	DashConfig.DashCurrentAmount = DashConfig.DashMaxAmount;

	if (InventoryComponent)
	{
		InventoryComponent->OnSeveredHeadAdded.AddUObject(this, &ABsCharacter::OnSeveredHeadPickup);
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ABsCharacter::Die);
	}

	if (const UCharacterMovementComponent* CharMovement = GetCharacterMovement())
	{
		SlideConfig.PreSlideGroundFriction = CharMovement->GroundFriction;
	}
	if (CameraComponent)
	{
		HeadBobConfig.InitialRelativeLocation = CameraComponent->GetRelativeLocation();
		OnDash.AddUObject(CameraComponent, &UBsCameraComponent::StartDashFOV);
		OnDashFinished.AddUObject(CameraComponent, &UBsCameraComponent::ResetFOV);

		OnSlideStart.AddUObject(CameraComponent, &UBsCameraComponent::StartSlideFOV);
		OnSlideStop.AddUObject(CameraComponent, &UBsCameraComponent::ResetFOV);
	}
}

float ABsCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (Weapon) 
	{
		Damage = Weapon->BlockIncomingDamage(Damage, DamageEvent, EventInstigator, DamageCauser);		
	}
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ABsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SlideTick(DeltaTime);
	DashTick(DeltaTime);

	// TODO: This will not need to exist once we have anim notifies.
	FootstepTick(DeltaTime);

	// TODO: We should move this to a timer. Not good to be putting everything into tick.
	ApplyWeaponSway(DeltaTime);
	MovementConfig.LastMovementVector = FVector2D::Zero();
	MovementConfig.LastLookVector = FVector2D::Zero();
}

// Called to bind functionality to input
void ABsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (bAlive)
		{
			//Jumping
			EnhancedInputComponent->BindAction(InputConfig.JumpAction, ETriggerEvent::Started, this, &ABsCharacter::Jump);
			EnhancedInputComponent->BindAction(InputConfig.JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			//Moving
			EnhancedInputComponent->BindAction(InputConfig.MoveAction, ETriggerEvent::Triggered, this, &ABsCharacter::Move);

			//Looking
			EnhancedInputComponent->BindAction(InputConfig.LookAction, ETriggerEvent::Triggered, this, &ABsCharacter::Look);

			// Dash
			EnhancedInputComponent->BindAction(InputConfig.DashAction, ETriggerEvent::Started, this, &ABsCharacter::Dash);

			// Attack
			EnhancedInputComponent->BindAction(InputConfig.AttackAction, ETriggerEvent::Triggered, this, &ABsCharacter::Attack);

			// Secondary Attack
			EnhancedInputComponent->BindAction(InputConfig.SecondaryAttackAction, ETriggerEvent::Started, this, &ABsCharacter::SecondaryAttack);

			// Grapple Hook
			EnhancedInputComponent->BindAction(InputConfig.GrappleAction, ETriggerEvent::Started, this, &ABsCharacter::UseGrappleHook);

			// Switch weapon attack mode
			EnhancedInputComponent->BindAction(InputConfig.AttackModeSwitchAction, ETriggerEvent::Started, this, &ABsCharacter::NextWeaponMode);

			// Interact
			EnhancedInputComponent->BindAction(InputConfig.InteractAction, ETriggerEvent::Started, this, &ABsCharacter::Interact);

			// Sliding
			EnhancedInputComponent->BindAction(InputConfig.SlideAction, ETriggerEvent::Started, this, &ABsCharacter::StartSliding);

			// Throw
			EnhancedInputComponent->BindAction(InputConfig.ThrowAction, ETriggerEvent::Started, this, &ABsCharacter::ThrowWeapon);

			// Detach head from weapon
			EnhancedInputComponent->BindAction(InputConfig.DetachHeadAction, ETriggerEvent::Started, this, &ABsCharacter::DetachHead);

			// Consume head on weapon
			EnhancedInputComponent->BindAction(InputConfig.ConsumeHeadAction, ETriggerEvent::Started, this, &ABsCharacter::ConsumeHead);
		}

		// Pause on button press
		EnhancedInputComponent->BindAction(InputConfig.PauseAction, ETriggerEvent::Started, this, &ABsCharacter::TogglePause);
	}
}

void ABsCharacter::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal,
	const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta)
{
	Super::OnWalkingOffLedge_Implementation(PreviousFloorImpactNormal, PreviousFloorContactNormal, PreviousLocation,
											TimeDelta);
	StartCoyoteTime();
}

void ABsCharacter::SetWeapon(ABsWeaponBase* InWeapon)
{
	Weapon = InWeapon;
	if (Weapon)
	{
		Weapon->SetOwner(this);
		Weapon->SetProjectileAimComponent(CameraComponent);
		Weapon->Equip();
		Weapon->OnWeaponCaught.AddUObject(this, &ABsCharacter::GrabCurrentWeapon);
		Weapon->OnWeaponParry.AddUObject(this, &ABsCharacter::OnParry);
		if (HealthComponent)
		{
			Weapon->OnHeal.AddDynamic(HealthComponent, &UBsHealthComponent::Heal);
		}
		OnWeaponChanged.Broadcast(Weapon);
	}
	if (UBsGrappleHookComponent* GrappleHookComponent = Weapon->FindComponentByClass<UBsGrappleHookComponent>())
	{
		GrappleHookComponent->OnGrappleHookAttached.AddDynamic(this, &ABsCharacter::StartGrapple);
		GrappleHookComponent->OnGrappleHookDetached.AddDynamic(this, &ABsCharacter::OnGrappleDetached);
		GrappleHookComponent->OnGrappleHookPull.AddUObject(this, &ABsCharacter::PullGrapple);
		GrappleHookComponent->SetEffectedCharacter(this); // TODO: Could replace with function bind that launched the character.
	}
	GrabCurrentWeapon();
}

void ABsCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	MovementConfig.LastMovementVector = MovementVector;
	if (Controller)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Pitch = 0.f;
		const FVector FacingDirection = Rotation.GetNormalized().Vector();

		const float MovementScale = SlideConfig.bSliding ? 0.3f : 1.f;
		
		AddMovementInput(FacingDirection, MovementVector.Y * MovementScale);
		AddMovementInput(GetActorRightVector(), MovementVector.X * MovementScale);
		if (CameraComponent)
		{
			CameraComponent->AddToRoll(MovementVector.X);
		}
	}
	OnCharacterSway.Broadcast(MovementConfig.LastMovementVector.X, MovementConfig.LastMovementVector.Y);
}

void ABsCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	MovementConfig.LastLookVector = LookAxisVector;

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABsCharacter::Jump()
{
	if (SlideConfig.bSliding)
	{
		OnSlideJump.Broadcast();
		StopSliding();
	}

	if (DashConfig.bDashing)
	{
		if (DashConfig.CanDashJump() && JumpCurrentCount < JumpMaxCount)
		{
			DashConfig.DepleteJumpDash();
			OnDashJump.Broadcast();
		}
		else
		{
			FinishDashing();
			Super::Jump();
			return;
		}
		StopDashing();
	}
	
	StopGrapple();

	// If the player recently walked off an edge, give them an extra time to jump.
	if (MovementConfig.bInCoyoteTime)
	{
		--JumpCurrentCount;
		MovementConfig.bInCoyoteTime = false;
	}
	Super::Jump();
}

void ABsCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	OnJump.Broadcast();
}

void ABsCharacter::Dash()
{
	if (!CanDash())
	{
		return;
	}

	StopGrapple();
	StopSliding();
	
	// Get input from Player, otherwise dash to our current direction
	FVector Direction = GetLastMovementInputVector();
	if (Direction.IsZero())
	{
		Direction = GetVelocity();
		Direction.Z = 0.f;
	}
	Direction.Normalize();

	if (Direction.IsZero())
	{
		// No Direction to dash in
		return;
	}
	
	Direction *= (GetCharacterMovement()->IsFalling() ? DashConfig.BaseDashStrength : DashConfig.GroundDashStrength);
	Direction.Z = 0.f; // No Dashing Up/Down

	DashConfig.DashDirection = Direction;
	DashConfig.PreDashVelocity = GetVelocity();
	DashConfig.bDashing = true;
	DashConfig.DashElapsedTime = 0.f;

	DashConfig.DepleteDash();
	OnDashAmountChanged.Broadcast();
	DashConfig.bDashEnabled = false;
	OnDashEnabledChanged.Broadcast();
	
	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		
		TimerManager.SetTimer(
			DashConfig.DashCooldownTimerHandle,
			this,
			&ABsCharacter::EnableDash,
			DashConfig.DashCooldown,
			false
		);

		TimerManager.ClearTimer(DashConfig.DashChargeTimerHandle);
		AddDashCharge();
		OnDash.Broadcast();
	}
}

// TODO: We probably don't need StopDashing and FinishDashing. We can probably just use FinishDashing.
void ABsCharacter::StopDashing()
{
	DashConfig.bDashing = false;
}

void ABsCharacter::FinishDashing()
{
	DashConfig.bDashing = false;
	DashConfig.DashElapsedTime = 0.f;
	const FVector DashNormal = DashConfig.DashDirection.GetSafeNormal();
	DashConfig.PreDashVelocity.Z = 0.f;
	GetCharacterMovement()->Velocity = DashNormal * GetCharacterMovement()->MaxWalkSpeed;
	/* Allows continued momentum after slide/dash jump. Leaving here in case we change our minds later.
	GetCharacterMovement()->Velocity = DashNormal * DashConfig.PreDashVelocity.Size(); */
	SetCanBeDamaged(true);
	OnDashFinished.Broadcast();
}

void ABsCharacter::DashTick(const float DeltaTime)
{
	SetCanBeDamaged(true);
	if (!DashConfig.bDashing)
	{
		return;
	}

	DashConfig.DashElapsedTime += DeltaTime;
	if (DashConfig.DashElapsedTime >= DashConfig.DashDuration)
	{
		FinishDashing();
		return;
	}	
	GetCharacterMovement()->Velocity = DashConfig.DashDirection;
	SetCanBeDamaged(false);
}

void ABsCharacter::FootstepTick(const float DeltaTime)
{
	if (DashConfig.bDashing || SlideConfig.bSliding || !GetCharacterMovement()->IsMovingOnGround())
	{
		MovementConfig.CurrentStrideDistance = 0;
		return;
	}

	const float Speed = GetVelocity().Size();
	if (Speed > 0)
	{
		MovementConfig.CurrentStrideDistance += Speed * DeltaTime;
		if (MovementConfig.CurrentStrideDistance >= MovementConfig.StrideLength)
		{
			MovementConfig.CurrentStrideDistance = 0;
			OnFootstep.Broadcast();
		}
	}
}

void ABsCharacter::EnableDash()
{
	DashConfig.bDashEnabled = true;
	OnDashEnabledChanged.Broadcast();
}

void ABsCharacter::AddDashCharge()
{
	DashConfig.DashChargeTimerHandle.Invalidate();
	if (DashConfig.DashCurrentAmount < DashConfig.DashMaxAmount)
	{
		const float DeltaTime = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
		DashConfig.DashCurrentAmount = FMath::Clamp(DashConfig.DashCurrentAmount + CalculateDashCharge() * DeltaTime, DashConfig.DashMinAmount, DashConfig.DashMaxAmount);
		OnDashAmountChanged.Broadcast();

		DashConfig.DashChargeTimerHandle = GetWorldTimerManager().SetTimerForNextTick(	
			this,
			&ABsCharacter::AddDashCharge
		);
	}
}

void ABsCharacter::RefundDashCharge()
{
	DashConfig.RefundDashCharge();
	OnDashAmountChanged.Broadcast();
}

float ABsCharacter::CalculateDashCharge()
{
	if (DashConfig.bDashing) return 0;
	float Modifier = GetCharacterMovement()->IsFalling() || SlideConfig.bSliding ? DashConfig.DashChargeModifier : 1.0f;
	return DashConfig.DashChargeAmount * Modifier;
}

bool ABsCharacter::CanDash()
{
	if (!DashConfig.bDashEnabled || DashConfig.DashCurrentAmount <= DashConfig.DashCost)
	{
		return false;
	}
	return true;
}

void ABsCharacter::StartGrapple()
{
	StopSliding();
	StopJumping();
	bGrappleAttached = true;
	GetWorldTimerManager().SetTimerForNextTick(this, &ABsCharacter::UseGrappleHook);
}

void ABsCharacter::StopGrapple()
{
	// TODO: There should be a way to "request detach" This should be done in the GrappleHookComponent.
	// TEMP HACK: Without this check we stack overflow.
	if (!bBeingPulledToGrapple) return;
	
	bGrappleAttached = false;
	bBeingPulledToGrapple = false;
	
	GetCharacterMovement()->ClearAccumulatedForces();

	if (Weapon)
	{
		if (UBsGrappleHookComponent* GrappleHookComponent = Weapon->FindComponentByClass<UBsGrappleHookComponent>())
		{
			GrappleHookComponent->DetachGrappleHook();
		}
	}
}

void ABsCharacter::OnGrappleDetached()
{
	bGrappleAttached = false;
	bBeingPulledToGrapple = false;
}

void ABsCharacter::PullGrapple(FVector Vector)
{
	if (bGrappleAttached)
	{
		LaunchCharacter(Vector, true, true);
	}
	else
	{
		StopGrapple();
	}
}

void ABsCharacter::Attack()
{
	if (Weapon)
	{
		Weapon->Fire();
	}
}

void ABsCharacter::SecondaryAttack()
{
	if (Weapon)
	{
		Weapon->SecondaryFire();
	}
}

void ABsCharacter::UseGrappleHook()
{
	if (bGrappleAttached && Weapon)
	{
		if (UBsGrappleHookComponent* GrappleHookComponent = Weapon->FindComponentByClass<UBsGrappleHookComponent>())
		{
			GrappleHookComponent->PullOwnerToLocation();
			bBeingPulledToGrapple = true;
		}
	}
}

void ABsCharacter::ThrowWeapon()
{
	if (Weapon)
	{
		StopGrapple();
		Weapon->Throw();
	}
}

void ABsCharacter::NextWeaponMode()
{
	if (Weapon)
	{
		Weapon->NextWeaponMode();
	}
}

void ABsCharacter::ApplyWeaponSway(const float DeltaTime)
{
	if (WeaponSpringArmComponent && Weapon)
	{
		// Movement sway
		const float HorizontalSwayAmount = MovementConfig.LastMovementVector.X;
		const FVector WeaponLocation = WeaponSpringArmComponent->GetRelativeLocation();
		WeaponSwayConfig.CurrentWeaponSwayAmount = WeaponSwayConfig.CalculateSwayAmount(DeltaTime,
			WeaponLocation.Y, HorizontalSwayAmount,
			WeaponSwayConfig.MaxHorizontalWeaponSwayAmount,
			WeaponSwayConfig.InitialWeaponLocation.Y,
			WeaponSwayConfig.WeaponMoveSwaySpeed
		);
		const FVector DesiredLocation = FVector(WeaponSwayConfig.InitialWeaponLocation.X, WeaponSwayConfig.CurrentWeaponSwayAmount, WeaponSwayConfig.InitialWeaponLocation.Z);
		WeaponSpringArmComponent->SetRelativeLocation(DesiredLocation);

		// Look sway
		const float VerticalSwayAmount = MovementConfig.LastLookVector.Y;
		const float RollSwayAmount = MovementConfig.LastLookVector.X;
		FRotator WeaponRotation = WeaponSpringArmComponent->GetRelativeRotation();

		WeaponRotation.Pitch = WeaponSwayConfig.CalculateHorizontalSwayAmount(DeltaTime, WeaponRotation.Pitch, VerticalSwayAmount);
		WeaponRotation.Roll = WeaponSwayConfig.CalculateVerticalSwayAmount(DeltaTime, WeaponRotation.Roll, RollSwayAmount);
		WeaponSpringArmComponent->SetRelativeRotation(WeaponRotation);
	}
}

void ABsCharacter::ApplyHeadBob_Implementation()
{
	
}

void ABsCharacter::DetachHead()
{
	if (Weapon)
	{
		Weapon->DetachSeveredHead();
	}
}

void ABsCharacter::ConsumeHead()
{
	if (Weapon)
	{
		Weapon->ConsumeSeveredHead();
	}
}

void ABsCharacter::Interact()
{
	if (const UWorld* World = GetWorld())
	{
		FHitResult InteractTraceResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		World->LineTraceSingleByChannel(
			InteractTraceResult,
			CameraComponent->GetComponentLocation(),
			CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * InteractConfig.InteractRange,
			ECollisionChannel::ECC_Visibility,
			QueryParams
		);

		if (InteractTraceResult.bBlockingHit)
		{
			if (IInteractable* Interactable = Cast<IInteractable>(InteractTraceResult.GetActor()))
			{
				Interactable->Interact(this);
			}

			if (ABsWeaponBase* HitWeapon = Cast<ABsWeaponBase>(InteractTraceResult.GetActor()))
			{
				if (Weapon != HitWeapon)
				{
					if (Weapon)	Weapon->Drop();
					SetWeapon(HitWeapon);
				}
			}
		}
	}
}

void ABsCharacter::StartSliding()
{
	if (SlideConfig.bSliding) return;
	
	if (GetVelocity().Size() >= SlideConfig.VelocityToStartSlide)
	{
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			// Can't slide mid-air
			if (MovementComponent->IsFalling()) return;
			
			SlideConfig.PreSlideGroundFriction = MovementComponent->GroundFriction;
			MovementComponent->GroundFriction = SlideConfig.SlideFriction;
		}
		
		SlideConfig.bSliding = true;

		if (UCapsuleComponent* Capsule = GetCapsuleComponent())
		{
			Capsule->SetCapsuleHalfHeight(SlideConfig.SlideHeight, true);
		}

		FVector Direction = GetVelocity();
		Direction.Z = 0.f;
		Direction.Normalize();
		LaunchCharacter(Direction * SlideConfig.SlideStrength, false, false);
		OnSlideStart.Broadcast();
	}
}

void ABsCharacter::StopSliding()
{
	if (SlideConfig.bSliding)
	{
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->GroundFriction = SlideConfig.PreSlideGroundFriction;
		}
		OnSlideStop.Broadcast();
	}
	
	SlideConfig.bSliding = false;
}

void ABsCharacter::StartCoyoteTime()
{
	MovementConfig.bInCoyoteTime = true;
	GetWorldTimerManager().SetTimer(
		MovementConfig.CoyoteTimeHandle,
		this,
		&ABsCharacter::EndCoyoteTime,
		MovementConfig.CoyoteTimeDuration,
		false
	);
}

void ABsCharacter::EndCoyoteTime()
{
	MovementConfig.bInCoyoteTime = false;
}

void ABsCharacter::OnParry()
{
	DashConfig.FillDashCharge();
	OnDashAmountChanged.Broadcast();
	StartHitStop();
}

void ABsCharacter::SlideTick(float DeltaTime)
{
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		const float DesiredHeight = SlideConfig.bSliding ? SlideConfig.SlideHeight :SlideConfig.PreSlideHeight;
		if (!FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), DesiredHeight))
		{
			const float NewHeight = FMath::FInterpTo(Capsule->GetUnscaledCapsuleHalfHeight(), DesiredHeight, DeltaTime, 5.f);
			Capsule->SetCapsuleHalfHeight(NewHeight, true);
		}
	}
	
	if (!SlideConfig.bSliding) return;
	
	if (GetVelocity().Size() < SlideConfig.VelocityToStopSlide)
	{	
		StopSliding();
	}
	
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		UWorld* World = GetWorld();
		if (World && MovementComp->IsFalling())
		{
			FHitResult HitResult;
			FCollisionQueryParams Params;
			TArray<AActor*> IgnoredActors;
			GetAttachedActors(IgnoredActors);
			Params.AddIgnoredActors(IgnoredActors);
			Params.AddIgnoredActor(this);
			
			World->LineTraceSingleByChannel(
				HitResult,
				GetActorLocation(),
				GetActorLocation() - FVector::UpVector * SlideConfig.SlopeSlideFloorDistance,
				ECollisionChannel::ECC_Visibility,
				Params
			);
			
			// If there is a slope below us, add a small impulse to keep sliding
			if (HitResult.bBlockingHit)
			{
				MovementComp->AddImpulse(FVector::UpVector * SlideConfig.SlopeSlideStrength, true);
			}
			else
			{
				// If we slide off a ledge, stop sliding
				StopSliding();
			}
		}
	}
}

void ABsCharacter::OnSeveredHeadPickup(ABsSeveredHeadBase* Head)
{
	if (Head)
	{
		if (UStaticMeshComponent* HeadMesh = Head->GetHeadMesh())
		{
			HeadMesh->SetSimulatePhysics(false);
			HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			HeadMesh->SetVisibility(false, true);
		}
		Head->SetActorEnableCollision(false);
		Head->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ABsCharacter::TogglePause()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		Unpause();
	}
	else
	{
		Pause();
	}
}

void ABsCharacter::Pause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	OnPaused.Broadcast();
}

void ABsCharacter::Unpause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	OnUnpaused.Broadcast();
}

void ABsCharacter::StartHitStop()
{
	GetWorldTimerManager().SetTimer(
		HitStopConfig.HitStopTimerHandle,
		this,
		&ABsCharacter::StopHitStop,
		HitStopConfig.HitStopDuration,
		false
	);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), HitStopConfig.HitStopTimeDilation);

	// Set relative time dilation to 50%
	CustomTimeDilation = 1 / HitStopConfig.HitStopTimeDilation * .5f;
}

void ABsCharacter::StopHitStop()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	CustomTimeDilation = 1.f;
}

void ABsCharacter::FellOutOfWorld(const UDamageType& DmgType)
{
	if (HealthComponent)
	{
		HealthComponent->OnDeath.Broadcast();
	}
}

void ABsCharacter::TriggerSecret() const
{
	OnSecretTriggered.Broadcast();
}

void ABsCharacter::Die()
{
	bAlive = false;

	if (Weapon)
	{
		Weapon->Drop();
	}

	if (InputComponent)
	{
		InputComponent->AxisBindings.Empty();
		InputComponent->ClearActionBindings();
		SetupPlayerInputComponent(InputComponent);
	}
}

void ABsCharacter::GrabCurrentWeapon()
{
	if (Weapon)
	{
		Weapon->AttachToComponent(WeaponSpringArmComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Weapon->SetActorRelativeRotation(FRotator(0.f, 180.f, 0.f));
		Weapon->SetActorRelativeLocation(FVector::ZeroVector);
		Weapon->Equip();
		WeaponSwayConfig.InitialWeaponLocation = WeaponSpringArmComponent->GetRelativeLocation();
		WeaponSwayConfig.InitialWeaponRotation = WeaponSpringArmComponent->GetRelativeRotation();
	}
}

void ABsCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (HeadBobConfig.MinimumVelocityThreshold < FMath::Abs(GetVelocity().Z))
	{
		ApplyHeadBob();

		if (!SlideConfig.bSliding && !DashConfig.bDashing)
		{
			OnLanded.Broadcast();
		}
	}

	if (DashConfig.bDashJumped)
	{
		DashConfig.bDashJumped = false;
		OnDashFinished.Broadcast();
	}
}
