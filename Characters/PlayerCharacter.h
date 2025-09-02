#pragma once

#include "CoreMinimal.h"
#include "GameCharacterBase.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInventoryComponent;
class UCameraMode;
class UPlayerCameraComponent;
class UPlayerAttributeSet;
class UPlayerHealthComponent;
class UBagManagerComponent;
class UInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class APlayGameHud;

USTRUCT(Blueprintable, BlueprintType)
struct FAbilityInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditAnywhere)
	FGameplayTag InputTag;
};

UCLASS()
class PROJECTPA_API APlayerCharacter : public AGameCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCameraComponent> PlayerCameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraMode> DefaultCameraMode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraMode> SwimmingCameraMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerAttributeSet> PlayerAttribute;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerHealthComponent> HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractionComponent> InteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultIMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PlayerPartWidgetAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<FAbilityInputAction> AbilityInputActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MaxSprintSpeed;

	UPROPERTY(Transient)
	TObjectPtr<APlayGameHud> CachedPlayGameHud;
	
public:
	APlayerCharacter();

	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual EGetItemError AddItem(FItemEncodedInfo& ItemToAdd) override { return EGetItemError::GeneralError; }
	virtual FItemEncodedInfo* GetItem(const FItemEncodedInfo& InItemEncodedInfo) override;
	virtual void RemoveItem(const FItemEncodedInfo& ItemToRemove) override;
	virtual void ForceAddItem(FItemEncodedInfo& ItemToAdd) override { }
	virtual void EquipItem(const FItemEncodedInfo& ItemToEquip, int32 ItemOwnerUID) override;
	virtual void UnEquipItem(FItemEncodedInfo& ItemToEquip) override { }
	virtual void WearItem(const FItemEncodedInfo& ItemToWear, int32 ItemOwnerUID) override;
	virtual void UnWearItem(FItemEncodedInfo& ItemToWear) override { }

	virtual bool IsSprinting() const override;
	FORCEINLINE UBagManagerComponent* GetBagComponent() const { return nullptr; }
	TSubclassOf<UCameraMode> GetPlayerCameraMode();
	
protected:
	virtual void BeginPlay() override;
	virtual void OnAbilitySystemReady() override;

private:
	void BindInputActions(UEnhancedInputComponent* EnhancedInputComponent);
	UFUNCTION()
	void OnTagInputPressed(FGameplayTag InputTag);
	UFUNCTION()
	void OnTagInputReleased(FGameplayTag InputTag);
	
	UFUNCTION(Server, Reliable)
	void ServerSetSprint(bool bSprint);
	void RequestServerSetSprint() { ServerSetSprint(true); }
	void RequestServerUnSetSprint() { ServerSetSprint(false); }

	UFUNCTION(Server, Reliable)
	void ServerSetCrouch(bool bCrouch);
	void RequestServerSetCrouch() { ServerSetCrouch(true); }
	void RequestServerUnSetCrouch() { ServerSetCrouch(false); }
	
	void Move(const FInputActionValue& Value);
	void CameraMove(const FInputActionValue& Value);
	void OnInteractionClicked(const FInputActionValue& Value);

	void OnInventoryPressed(const FInputActionValue& Value);
	void OnPlayerPartWidgetPressed(const FInputActionValue& Value);

	virtual void StopAllActions() override;
	
	virtual void StartDying();
	virtual void DyingEnd();

	APlayGameHud* GetPlayGameHud();
};

