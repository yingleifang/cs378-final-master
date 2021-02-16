// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

class AGun;

UENUM(BlueprintType)
enum class ECharacterActionStateEnum : uint8 {
	IDLE UMETA(DisplayName = "Idling"),
	MOVE UMETA(DisplayName = "Moving"),
	JUMP UMETA(DisplayName = "Jumping"),
	CROUCH UMETA(DisplayName = "Crouching"),
	PICKING UMETA(DisplayName = "PickingUp"),
	PULLING UMETA(DisplayName = "Pulling")
};

UCLASS(Blueprintable)
class CS378_FINAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<int32> Inventory;
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UFUNCTION(BlueprintImplementableEvent)
		void MoveForward(float value);

	UFUNCTION(BlueprintImplementableEvent)
		void MoveRight(float value);

	UFUNCTION(BlueprintImplementableEvent)
		void JumpInit();

	UFUNCTION(BlueprintImplementableEvent)
		void StartSprint();

	UFUNCTION(BlueprintImplementableEvent)
		void EndSprint();

	UFUNCTION(BlueprintImplementableEvent)
		void StartCrouch();

	UFUNCTION(BlueprintImplementableEvent)
		void EndCrouch();

	UFUNCTION(BlueprintImplementableEvent)
		void Interact();

	UFUNCTION(BlueprintImplementableEvent)
		void InteractFinalize();

	UFUNCTION(BlueprintImplementableEvent)
		void Shoot(bool isDestroy);

	void RotateSpringArm(float pitch, float yaw, float roll);


	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY()
		UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float InteractTime;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float ShootCooldown;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float SpringArmMaxPitch;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ECharacterActionStateEnum CharacterActionState;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float SprintMod;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		bool CanShoot = true;

	UFUNCTION(BlueprintCallable)
		bool CanPerformAction(ECharacterActionStateEnum updatedAction);

	UFUNCTION(BlueprintCallable)
		void UpdateActionState(ECharacterActionStateEnum newAction);

	UFUNCTION(BlueprintCallable)
		void DoMoveForward(float scale);

	UFUNCTION(BlueprintCallable)
		void DoMoveRight(float scale);

	UFUNCTION(BlueprintCallable)
		void PerformInteraction();

	UFUNCTION(BlueprintCallable)
		void FinalizeInteraction();

	UFUNCTION(BlueprintCallable)
		void JumpAction();

	UFUNCTION(BlueprintCallable)
		void FinalizeJump();

	UFUNCTION(BlueprintCallable)
		void InteractEnd();

	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	void StoreBlock(int32 block);

	bool UseBlock(int32 block);

	float shootCooldownTime = 0.f;
	float rayTraceDistance;

	int currentItem = 1; // What block the character is currently holding: default = 1 -> dirt from enum

	FHitResult enemy;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	TArray<int32> GetInventory();
};
