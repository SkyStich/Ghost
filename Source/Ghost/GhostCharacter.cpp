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
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Player/ModesOfMovementPlayerComponent.h"

AGhostCharacter::AGhostCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bReplicates = true;
	NetUpdateFrequency = 30.f;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	DoorTurnRate = 2.f;

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
	FollowCamera->SetupAttachment(GetMesh(), "Head"); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	/** Create storage component */
	StoragePlayerComponent = CreateDefaultSubobject<UStoragePlayerComponent>(TEXT("StorageComponent"));

	/** Create stimuli component */
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliComponent"));

	ModesOfMovementComponent = CreateDefaultSubobject<UModesOfMovementPlayerComponent>(TEXT("ModesOfMovementComponent"));
}

void AGhostCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("DoorInteraction", IE_Pressed, this, &AGhostCharacter::DoorInteractionPressed);
	PlayerInputComponent->BindAction("DoorInteraction", IE_Released, this, &AGhostCharacter::DropInteractionReleased);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, ModesOfMovementComponent, &UModesOfMovementPlayerComponent::OwnerStartUseStamina);
	PlayerInputComponent->BindAction("Sprint", IE_Released, ModesOfMovementComponent, &UModesOfMovementPlayerComponent::OwnerStopUseStamina);
	
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &AGhostCharacter::PressedInteraction);

	PlayerInputComponent->BindAction("UseItemDirectly", IE_Released, this, &AGhostCharacter::UseItemDirectlyPressed);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGhostCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGhostCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(	"TurnRate", this, &AGhostCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGhostCharacter::LookUpAtRate);
}

void AGhostCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGhostCharacter, PlayerTurn, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AGhostCharacter, InteractionDoor, COND_SkipOwner);
}

void AGhostCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val);

	UpdateDoorRotate(Val);
}

void AGhostCharacter::BeginPlay()
{
	Super::BeginPlay();

	StoragePlayerComponent->OnNewCurrentItem.AddDynamic(this, &AGhostCharacter::OnNewCurrentWeaponEvent);
	StoragePlayerComponent->OnRemoveItem.AddDynamic(this, &AGhostCharacter::OnItemRemoveEvent);
	ModesOfMovementComponent->OnStaminaUseChanged.AddDynamic(this, &AGhostCharacter::OnUseStaminaEvent);
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

		/** update move forward */
		if(Value > 0)
		{
			bMoveForward = true;
		}
		else if(Value < 0 && bMoveForward)
		{
			bMoveForward = false;
			if(ModesOfMovementComponent->GetStaminaUse()) ModesOfMovementComponent->OwnerStopUseStamina();
		}
		return;
	}
	bMoveForward = false;
	if(ModesOfMovementComponent->GetStaminaUse()) ModesOfMovementComponent->OwnerStopUseStamina();
}

void AGhostCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);

		if(bMoveForward)
		{
			bMoveForward = false;
			if(ModesOfMovementComponent->GetStaminaUse()) ModesOfMovementComponent->OwnerStopUseStamina();
		}
	}
}

void AGhostCharacter::OnNewCurrentWeaponEvent(ABaseItem* NewItem)
{
	if(NewItem)
	{
		NewItem->AddToStorage();

		if(!NewItem->GetAttachParentActor())
		NewItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SKT_RightArmItem");
	}
}

void AGhostCharacter::OnItemRemoveEvent(ABaseItem* NewItem)
{
	/** todo: replace the impulse with a more controlled logic of the throw */
	if(Controller)
		NewItem->GetStaticMeshComponent()->AddImpulse(Controller->GetControlRotation().Vector() * 400.f);
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

void AGhostCharacter::UseItemDirectlyPressed()
{
	auto const TempItem = StoragePlayerComponent->GetPlayerCurrentItem();
	if(TempItem)
	{
		TempItem->UseItemDirectlyPressed();
	}
}

bool AGhostCharacter::DropInteractionDoorTrace(FHitResult& OutHit)
{
	FVector const TraceStart = FollowCamera->GetComponentLocation();
	FVector const TraceEnd = Controller->GetControlRotation().Vector() * 400.f + TraceStart;
	
	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECC_GameTraceChannel2);
	Params.AddObjectTypesToQuery(ECC_WorldStatic);
	Params.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	return GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, Params);
}

void AGhostCharacter::DropInteractionReleased()
{
	if(InteractionDoor)
	{
		InteractionDoor = nullptr;
		Server_FinishDoorInteraction();
	}
}

void AGhostCharacter::DoorInteractionPressed()
{
	FHitResult OutHit;
	bool const bResult = DropInteractionDoorTrace(OutHit);

	if(bResult && OutHit.GetActor() && OutHit.GetActor()->ActorHasTag("Door"))
	{
		InteractionDoor = OutHit.GetComponent();
		Server_DoorInteractionTrace();
	}
}

void AGhostCharacter::Server_DoorInteractionTrace_Implementation()
{
	FHitResult OutHit;
	bool const bResult = DropInteractionDoorTrace(OutHit);

	if(bResult && OutHit.GetActor() && OutHit.GetActor()->ActorHasTag("Door"))
	{
		InteractionDoor = OutHit.GetComponent();
	}
}

void AGhostCharacter::Server_FinishDoorInteraction_Implementation()
{
	InteractionDoor = nullptr;
}

void AGhostCharacter::UpdateDoorRotate(float Rate)
{
	if(InteractionDoor && Rate != 0.f && Controller)
	{
		PlayerTurn = Rate * DoorTurnRate * -1;
		OnRep_PlayerTurn();
		
		Server_UpdateDoorRotate(PlayerTurn);
	}
}

void AGhostCharacter::Server_UpdateDoorRotate_Implementation(float Rate)
{
	if(InteractionDoor)
	{
		PlayerTurn = Rate;
		OnRep_PlayerTurn();
	}
}

void AGhostCharacter::OnRep_PlayerTurn()
{
	if(InteractionDoor)
	{
		FQuat const CombineRotation = FQuat(InteractionDoor->GetComponentRotation()) * FQuat(FRotator(0.f, PlayerTurn, 0.f));
		InteractionDoor->SetWorldRotation(FRotator(CombineRotation));
	}
}

void AGhostCharacter::OnUseStaminaEvent(bool bState)
{
	/** continue only if this local controller of server */
	if(!Controller) return;
	
	if(bState)
	{
		GetCharacterMovement()->MaxWalkSpeed = ModesOfMovementComponent->GetMovementParam().MaxSprintSpeed;
	}
	else
	{	
		GetCharacterMovement()->MaxWalkSpeed = ModesOfMovementComponent->GetMovementParam().MaxRunSpeed;
	}
}
