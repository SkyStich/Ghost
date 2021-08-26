// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/Player/StoragePlayerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Actors/Items/Base/BaseItem.h"
#include "DrawDebugHelpers.h"

AGhostCharacter::AGhostCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bReplicates = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.1f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "head"); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	StoragePlayerComponent = CreateDefaultSubobject<UStoragePlayerComponent>(TEXT("StorageComponent"));
}

void AGhostCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &AGhostCharacter::PressedInteraction);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGhostCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGhostCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGhostCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGhostCharacter::LookUpAtRate);
}

void AGhostCharacter::BeginPlay()
{
	Super::BeginPlay();

	StoragePlayerComponent->OnNewCurrentItem.AddDynamic(this, &AGhostCharacter::OnNewCurrentWeaponEvent);
}

void AGhostCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGhostCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGhostCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGhostCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AGhostCharacter::OnNewCurrentWeaponEvent(ABaseItem* NewItem)
{
	if(NewItem)
	{
		NewItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "SKT_RightArmItem");
	}
}

AActor* AGhostCharacter::DropLineTraceFromInteraction()
{
	FVector const TraceStart = FollowCamera->GetComponentLocation();
	FVector const TraceEnd = Controller->GetControlRotation().Vector() * 550.f + TraceStart;

	FHitResult OutHit;

	GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECC_Visibility);

	return OutHit.GetActor();
}

void AGhostCharacter::PressedInteraction()
{
	FVector const Start = FollowCamera->GetComponentLocation();
	FVector const End = Controller->GetControlRotation().Vector() * 550.f + Start;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 4.f, 5);
	
	auto const ActorForInteraction = DropLineTraceFromInteraction();
	if(!ActorForInteraction) return;
	if(!ActorForInteraction->GetClass()->ImplementsInterface(UPlayerItemInteraction::StaticClass())) return;
	Server_InteractionWithItem();
}

void AGhostCharacter::Server_InteractionWithItem_Implementation()
{
	auto const ActorForInteraction = DropLineTraceFromInteraction();
	
	FVector const Start = FollowCamera->GetComponentLocation();
	FVector const End = Controller->GetControlRotation().Vector() * 550.f + Start;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 4.f, 5);

	if(!ActorForInteraction) return;
	if(!ActorForInteraction->GetClass()->ImplementsInterface(UPlayerItemInteraction::StaticClass())) return;

	IPlayerItemInteraction::Execute_PlayerInteractionWithItem(ActorForInteraction, this);
}

