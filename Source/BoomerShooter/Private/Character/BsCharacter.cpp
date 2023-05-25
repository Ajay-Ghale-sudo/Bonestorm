// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BsCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/BsHealthComponent.h"
#include "Component/BsInventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Interactable.h"
#include "Weapon/BsWeaponBase.h"

// Sets default values
ABsCharacter::ABsCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraComponent->bUsePawnControlRotation = true;

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

	DashConfig.DashCharges = DashConfig.MaxDashCharges;
}

// Called every frame
void ABsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SlideTick(DeltaTime);
}

// Called to bind functionality to input
void ABsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		//Jumping
		EnhancedInputComponent->BindAction(InputConfig.JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(InputConfig.JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(InputConfig.MoveAction, ETriggerEvent::Triggered, this, &ABsCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(InputConfig.LookAction, ETriggerEvent::Triggered, this, &ABsCharacter::Look);

		// Dash
		EnhancedInputComponent->BindAction(InputConfig.DashAction, ETriggerEvent::Started, this, &ABsCharacter::Dash);

		// Attack
		EnhancedInputComponent->BindAction(InputConfig.AttackAction, ETriggerEvent::Started, this, &ABsCharacter::Attack);

		// Switch weapon attack mode
		EnhancedInputComponent->BindAction(InputConfig.AttackModeSwitchAction, ETriggerEvent::Started, this, &ABsCharacter::NextWeaponMode);

		// Interact
		EnhancedInputComponent->BindAction(InputConfig.InteractAction, ETriggerEvent::Started, this, &ABsCharacter::Interact);

		// Sliding
		EnhancedInputComponent->BindAction(InputConfig.SlideAction, ETriggerEvent::Started, this, &ABsCharacter::StartSliding);
	}
}

void ABsCharacter::SetWeapon(ABsWeaponBase* InWeapon)
{
	Weapon = InWeapon;
	if (Weapon)
	{
		Weapon->SetOwner(this);
	}
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
	Super::Jump();
	StopSliding();
}

void ABsCharacter::Dash()
{
	if (!DashConfig.bDashEnabled || DashConfig.DashCharges <= 0)
	{
		return;
	}

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

	LaunchCharacter(Direction, false, false);
	DashConfig.DashCharges--;
	OnDashAmountChanged.Broadcast();
	
	DashConfig.bDashEnabled = false;
	
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
		TimerManager.SetTimer(
			DashConfig.DashChargeTimerHandle,
			this,
			&ABsCharacter::AddDashCharge,
			DashConfig.DashChargeRate,
			false
		);
	}
}

void ABsCharacter::EnableDash()
{
	DashConfig.bDashEnabled = true;
}

void ABsCharacter::AddDashCharge()
{
	if (DashConfig.DashCharges < DashConfig.MaxDashCharges)
	{
		DashConfig.DashCharges++;
		OnDashAmountChanged.Broadcast();
	}

	if (DashConfig.DashCharges < DashConfig.MaxDashCharges)
	{
		GetWorldTimerManager().SetTimer(	
			DashConfig.DashChargeTimerHandle,
			this,
			&ABsCharacter::AddDashCharge,
			DashConfig.DashChargeRate,
			false
		);
	}
}

void ABsCharacter::Attack()
{
	if (Weapon)
	{
		Weapon->Fire();
	}
}

void ABsCharacter::NextWeaponMode()
{
	if (Weapon)
	{
		Weapon->NextWeaponMode();
	}
}

void ABsCharacter::Interact()
{
	if (const UWorld* World = GetWorld())
	{
		FHitResult InteractTraceResult;
		World->LineTraceSingleByChannel(
			InteractTraceResult,
			CameraComponent->GetComponentLocation(),
			CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * InteractConfig.InteractRange,
			ECollisionChannel::ECC_Visibility
		);

		if (InteractTraceResult.bBlockingHit)
		{
			if (IInteractable* Interactable = Cast<IInteractable>(InteractTraceResult.GetActor()))
			{
				Interactable->Interact(this);
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
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->GroundFriction = SlideConfig.PreSlideGroundFriction;
	}
	
	SlideConfig.bSliding = false;
}

void ABsCharacter::SlideTick(float DeltaTime)
{
	if (SlideConfig.bSliding && GetVelocity().Size() < SlideConfig.VelocityToStopSlide)
	{	
		StopSliding();
	}
	
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		const float DesiredHeight = SlideConfig.bSliding ? SlideConfig.SlideHeight :SlideConfig.PreSlideHeight;
		if (!FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), DesiredHeight))
		{
			const float NewHeight = FMath::FInterpTo(Capsule->GetUnscaledCapsuleHalfHeight(), DesiredHeight, DeltaTime, 5.f);
			Capsule->SetCapsuleHalfHeight(NewHeight, true);
		}

	}
}

void ABsCharacter::SetHealth(float Value)
{
	
}



