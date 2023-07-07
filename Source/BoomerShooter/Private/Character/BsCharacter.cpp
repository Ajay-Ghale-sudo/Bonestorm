// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BsCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/BsGrappleHookComponent.h"
#include "Component/BsHealthComponent.h"
#include "Component/BsInventoryComponent.h"
#include "Component/Camera/BsCameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Interactable.h"
#include "Props/Head/BsSeveredHeadBase.h"
#include "Weapon/BsWeaponBase.h"
#include "Weapon/Scythe/BsScythe.h"

// Sets default values
ABsCharacter::ABsCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UBsCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraComponent->bUsePawnControlRotation = false;

	WeaponSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("WeaponSpringArm"));
	WeaponSpringArmComponent->SetupAttachment(CameraComponent);
	WeaponSpringArmComponent->bDoCollisionTest = false;
	WeaponSpringArmComponent->TargetArmLength = 60.f;

	InventoryComponent = CreateDefaultSubobject<UBsInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UBsHealthComponent>(TEXT("HealthComponent"));
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
			EnhancedInputComponent->BindAction(InputConfig.SecondaryAction, ETriggerEvent::Started, this, &ABsCharacter::SecondaryAttack);

			// Switch weapon attack mode
			EnhancedInputComponent->BindAction(InputConfig.AttackModeSwitchAction, ETriggerEvent::Started, this, &ABsCharacter::NextWeaponMode);

			// Interact
			EnhancedInputComponent->BindAction(InputConfig.InteractAction, ETriggerEvent::Started, this, &ABsCharacter::Interact);

			// Sliding
			EnhancedInputComponent->BindAction(InputConfig.SlideAction, ETriggerEvent::Started, this, &ABsCharacter::StartSliding);

			// Throw
			EnhancedInputComponent->BindAction(InputConfig.ThrowAction, ETriggerEvent::Started, this, &ABsCharacter::ThrowWeapon);

			// Block
			EnhancedInputComponent->BindAction(InputConfig.BlockAction, ETriggerEvent::Started, this, &ABsCharacter::StartBlock);
			EnhancedInputComponent->BindAction(InputConfig.BlockAction, ETriggerEvent::Completed, this, &ABsCharacter::StopBlock);

			// Detach head from weapon
			EnhancedInputComponent->BindAction(InputConfig.DetachHeadAction, ETriggerEvent::Started, this, &ABsCharacter::DetachHead);

			// Consume head on weapon
			EnhancedInputComponent->BindAction(InputConfig.ConsumeHeadAction, ETriggerEvent::Started, this, &ABsCharacter::ConsumeHead);
		}

		// Pause on button press
		EnhancedInputComponent->BindAction(InputConfig.PauseAction, ETriggerEvent::Started, this, &ABsCharacter::Pause);
	}
}

void ABsCharacter::SetWeapon(ABsWeaponBase* InWeapon)
{
	Weapon = InWeapon;
	if (Weapon)
	{
		Weapon->SetOwner(this);
		Weapon->Equip();
		Weapon->OnWeaponCaught.AddUObject(this, &ABsCharacter::GrabCurrentWeapon);
		Weapon->OnWeaponParry.AddUObject(this, &ABsCharacter::RefundDashCharge);
		if (HealthComponent)
		{
			Weapon->OnHeal.AddDynamic(HealthComponent, &UBsHealthComponent::Heal);
		}
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
	if (Controller)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Pitch = 0.f;
		const FVector FacingDirection = Rotation.GetNormalized().Vector();

		float MovementScale = SlideConfig.bSliding ? 0.3f : 1.f;
		
		AddMovementInput(FacingDirection, MovementVector.Y * MovementScale);
		AddMovementInput(GetActorRightVector(), MovementVector.X * MovementScale);

		if (CameraComponent)
		{
			CameraComponent->AddToRoll(MovementVector.X);
		}
	}
}

void ABsCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABsCharacter::Jump()
{
	StopDashing();
	StopSliding();
	StopGrapple();

	Super::Jump();
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
	}
	Direction.Normalize();
	
	Direction *= (GetCharacterMovement()->IsFalling() ? DashConfig.BaseDashStrength : DashConfig.GroundDashStrength);
	Direction.Z = 0.f; // No Dashing Up/Down

	DashConfig.DashDirection = Direction;
	DashConfig.PreDashVelocity = GetVelocity();
	DashConfig.bDashing = true;
	DashConfig.DashElapsedTime = 0.f;

	DashConfig.DashCurrentAmount -= DashConfig.DashCost;
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
	}
}

void ABsCharacter::StopDashing()
{
	DashConfig.bDashing = false;
}

void ABsCharacter::DashTick(const float DeltaTime)
{
	SetCanBeDamaged(true);
	if (!DashConfig.bDashing || DashConfig.DashElapsedTime >= DashConfig.DashDuration)
	{
		return;
	}

	DashConfig.DashElapsedTime += DeltaTime;
	GetCharacterMovement()->Velocity = DashConfig.DashDirection;
	SetCanBeDamaged(false);

	if (DashConfig.DashElapsedTime >= DashConfig.DashDuration)
	{
		DashConfig.bDashing = false;
		DashConfig.DashElapsedTime = 0.f;
		FVector DashNormal = DashConfig.DashDirection;
		DashNormal.Normalize();
		GetCharacterMovement()->Velocity = DashNormal * DashConfig.PreDashVelocity.Length();
		SetCanBeDamaged(true);
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
		DashConfig.DashCurrentAmount = FMath::Clamp(DashConfig.DashCurrentAmount + DashConfig.DashChargeAmount * DeltaTime, DashConfig.DashMinAmount, DashConfig.DashMaxAmount);
		OnDashAmountChanged.Broadcast();

		DashConfig.DashChargeTimerHandle = GetWorldTimerManager().SetTimerForNextTick(	
			this,
			&ABsCharacter::AddDashCharge
		);
	}
}

void ABsCharacter::RefundDashCharge()
{
	DashConfig.DashCurrentAmount = FMath::Clamp(DashConfig.DashCurrentAmount + DashConfig.DashCost, DashConfig.DashMinAmount, DashConfig.DashMaxAmount);
	OnDashAmountChanged.Broadcast();
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

void ABsCharacter::StartBlock()
{
	if (Weapon)
	{
		Weapon->StartBlock();
	}
}

void ABsCharacter::StopBlock()
{
	if (Weapon)
	{
		Weapon->StopBlock();
	}
}

void ABsCharacter::SecondaryAttack()
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
	}
	
	SlideConfig.bSliding = false;
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

void ABsCharacter::Pause()
{
	OnPaused.Broadcast();
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
	}
}