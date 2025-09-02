#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "CharacterAttributes/PlayerAttributeSet.h"
#include "Controllers/IngamePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inputs/PAInputComponent.h"
#include "ProjectPA/Camera/PlayerCameraComponent.h"
#include "ProjectPA/Components/EquipmentComponent.h"
#include "ProjectPA/Components/InteractionComponent.h"
#include "ProjectPA/Components/InventoryComponent.h"
#include "ProjectPA/Components/PlayerAbilitySystemComponent.h"
#include "ProjectPA/Components/PlayerHealthComponent.h"
#include "ProjectPA/HUD/PlayGameHud.h"
#include "ProjectPA/Items/ItemBase.h"
#include "ProjectPA/Widgets/SubSystem/PAViewModelUtils.h"
#include "ProjectPA/Widgets/ViewModel/ItemListViewModel.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("HealthComponent"));
	PlayerAttribute = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Player Attribute Set"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
	InteractionComponent->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	MaxWalkSpeed = 300.0f;
	MaxSprintSpeed = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	PlayerCameraComponent = CreateDefaultSubobject<UPlayerCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerCameraComponent->SetupAttachment(GetRootComponent());
	PlayerCameraComponent->bUsePawnControlRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("AbilityComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

TSubclassOf<UCameraMode> APlayerCharacter::GetPlayerCameraMode()
{
	return bIsSwimming ? SwimmingCameraMode : DefaultCameraMode;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultIMC, 0);
		}
	}

	if (PlayerCameraComponent)
	{
		PlayerCameraComponent->DetermineCameraModeDelegate.BindUObject(this, &APlayerCharacter::GetPlayerCameraMode);
	}

	// Add ItemListViewModel for widgets
	UItemListViewModel* ItemListViewModel = NewObject<UItemListViewModel>(this);
	if (IsValid(ItemListViewModel))
	{
		ItemListViewModel->Initialize(this);
		PAViewModelUtils::AddViewModel(GetOwner(), ItemListViewModel);
	}
}

void APlayerCharacter::OnAbilitySystemReady()
{
	Super::OnAbilitySystemReady();

	if (HealthComponent && PlayerAttribute)
	{
		PlayerAttribute->HealthAttributeChangedEvent.AddUObject(HealthComponent, &UPlayerHealthComponent::OnHealthAttributeChanged);
		PlayerAttribute->MaxHealthAttributeChangedEvent.AddUObject(HealthComponent, &UPlayerHealthComponent::OnMaxHealthAttributeChanged);
		PlayerAttribute->MoveSpeedAttributeChangedEvent.AddUObject(HealthComponent, &UPlayerHealthComponent::OnMoveSpeedAttributeChanged);

		PlayerAttribute->OnHealthChangedEvent.AddUObject(HealthComponent, &UPlayerHealthComponent::OnHealthChanged);
		PlayerAttribute->OnStaminaChangedEvent.AddUObject(HealthComponent, &UPlayerHealthComponent::OnStaminaChanged);

		PlayerAttribute->OnBodyPartChanged.AddUObject(HealthComponent, &UPlayerHealthComponent::SetPartValue);
		
		HealthComponent->OnPlayerMovedEvent.AddUObject(PlayerAttribute, &UPlayerAttributeSet::OnPlayerMoved);
		
		HealthComponent->SetupHealthWidget();
		PlayerAttribute->BindHealthComponentEnd();
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::CameraMove);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &APlayerCharacter::OnInteractionClicked);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::RequestServerSetSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::RequestServerUnSetSprint);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::RequestServerSetCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::RequestServerUnSetCrouch);
		
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::OnInventoryPressed);
		EnhancedInputComponent->BindAction(PlayerPartWidgetAction, ETriggerEvent::Started, this, &APlayerCharacter::OnPlayerPartWidgetPressed);

		BindInputActions(EnhancedInputComponent);
	}
}

void APlayerCharacter::BindInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	UPAInputComponent* PAInputComponent = Cast<UPAInputComponent>(EnhancedInputComponent);
	if (IsValid(PAInputComponent))
	{
		for (const FAbilityInputAction& AbilityInputAction : AbilityInputActions)
		{
			if (IsValid(AbilityInputAction.InputAction) && AbilityInputAction.InputTag.IsValid())
			{
				PAInputComponent->BindAbilityActions(AbilityInputAction.InputAction, this, &APlayerCharacter::OnTagInputPressed, &APlayerCharacter::OnTagInputReleased, AbilityInputAction.InputTag);
			}
		}
	}
}

void APlayerCharacter::OnTagInputPressed(FGameplayTag InputTag)
{
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(GetAbilitySystemComponent());
	if (IsValid(ASC))
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void APlayerCharacter::OnTagInputReleased(FGameplayTag InputTag)
{
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(GetAbilitySystemComponent());
	if (IsValid(ASC))
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bCharacterDeadEnd || bCharacterDeadStart)
	{
		return;
	}
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::CameraMove(const FInputActionValue& Value)
{
	const auto LookAxisValue = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void APlayerCharacter::ServerSetSprint_Implementation(bool bSprint)
{
	PlayerAttribute->SetSprinting(bSprint);
}

void APlayerCharacter::ServerSetCrouch_Implementation(bool bCrouch)
{
	PlayerAttribute->SetCrouching(bCrouch);
}

void APlayerCharacter::OnInteractionClicked(const FInputActionValue& Value)
{
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract(this);
	}
}

void APlayerCharacter::OnInventoryPressed(const FInputActionValue& Value)
{
	if (APlayGameHud* HUD = GetPlayGameHud())
	{
		HUD->ToggleInventoryWidget();
	}
}

void APlayerCharacter::OnPlayerPartWidgetPressed(const FInputActionValue& Value)
{
	if (HealthComponent)
	{
		HealthComponent->ToggleStatusPartWidget();
	}
}

void APlayerCharacter::StopAllActions()
{
	Super::StopAllActions();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
	}
}

FItemEncodedInfo* APlayerCharacter::GetItem(const FItemEncodedInfo& InItemEncodedInfo)
{
	if (InventoryComponent)
	{
		InventoryComponent->GetItem(InItemEncodedInfo);
	}
	return nullptr;
}

void APlayerCharacter::RemoveItem(const FItemEncodedInfo& ItemToRemove)
{
	Super::RemoveItem(ItemToRemove);

	if (InventoryComponent)
	{
		InventoryComponent->RemoveItem(ItemToRemove);
	}
}

void APlayerCharacter::EquipItem(const FItemEncodedInfo& ItemToEquip, const int32 ItemOwnerUID)
{
	Super::EquipItem(ItemToEquip, ItemOwnerUID);

	if (IsValid(EquipmentComponent))
	{
		EquipmentComponent->EquipItem(ItemToEquip, ItemOwnerUID);
	}
}

void APlayerCharacter::WearItem(const FItemEncodedInfo& ItemToWear, const int32 ItemOwnerUID)
{
	Super::WearItem(ItemToWear, ItemOwnerUID);

	if (IsValid(EquipmentComponent))
	{
		EquipmentComponent->WearItem(ItemToWear, ItemOwnerUID);
	}
}

bool APlayerCharacter::IsSprinting() const
{
	return IsValid(PlayerAttribute) ? PlayerAttribute->IsSprinting() : Super::IsSprinting();
}

void APlayerCharacter::StartDying()
{
	Super::StartDying();

	if (AIngamePlayerController* PlayerController = Cast<AIngamePlayerController>(GetController()))
	{
		DisableInput(PlayerController);
		PlayerController->DisableInput(PlayerController);
	}

	DyingEnd();
}

void APlayerCharacter::DyingEnd()
{
	Super::DyingEnd();
}

APlayGameHud* APlayerCharacter::GetPlayGameHud()
{
	if (CachedPlayGameHud == nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			CachedPlayGameHud = Cast<APlayGameHud>(PC->GetHUD());
		}
	}
	return CachedPlayGameHud;
}
