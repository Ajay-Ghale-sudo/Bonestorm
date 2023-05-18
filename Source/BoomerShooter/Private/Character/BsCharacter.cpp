// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BsCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	JumpMaxCount = 2;
}

// Called every frame
void ABsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		AddMovementInput(FacingDirection, MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);		
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
}

void ABsCharacter::Dash()
{
	if (!DashConfig.bDashEnabled)
	{
		return;
	}
	
	// Get input from Player, otherwise dash to our current direction
	FVector Direction = GetLastMovementInputVector();
	if (Direction.IsZero())
	{
		Direction = GetVelocity();
	}
	Direction.Normalize();
	
	Direction *= (GetCharacterMovement()->IsFalling() ? DashConfig.BaseDashStrength : DashConfig.GroundDashStrength);
	Direction.Z = 0.f; // No Dashing Up/Down

	LaunchCharacter(Direction, true, true);

	DashConfig.bDashEnabled = false;
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			DashConfig.DashCooldownTimerHandle,
			this,
			&ABsCharacter::EnableDash,
			DashConfig.DashCooldown,
			false
		);
	}
}

void ABsCharacter::EnableDash()
{
	DashConfig.bDashEnabled = true;
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



