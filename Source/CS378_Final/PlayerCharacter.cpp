// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

#include <string> 
#include "Gun.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractTime = 3.f;
	ShootCooldown = 0.5f;
	SpringArmMaxPitch = 70.f;
	SprintMod = 8.f;
	TimerHandle = FTimerHandle();

	// Camera stuff
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	rayTraceDistance = 1000.f;

	enemy = FHitResult();

	
	Inventory.Init(0, 4);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Cast<UCapsuleComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanShoot) {
		shootCooldownTime -= DeltaTime;
		if (shootCooldownTime < 0.f) {
			shootCooldownTime = ShootCooldown;
			CanShoot = true;
		}
	}
}

// Called to bind functionality to input 
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


bool APlayerCharacter::CanPerformAction(ECharacterActionStateEnum updatedAction)
{
	switch (CharacterActionState)
	{
	case ECharacterActionStateEnum::IDLE:
		return true;
		break;

	case ECharacterActionStateEnum::MOVE:
		if (updatedAction != ECharacterActionStateEnum::PICKING
			&& updatedAction != ECharacterActionStateEnum::PULLING) {
			return true;
		}
		break;

	case ECharacterActionStateEnum::JUMP:
		if (updatedAction == ECharacterActionStateEnum::IDLE
			|| updatedAction == ECharacterActionStateEnum::MOVE
			|| updatedAction == ECharacterActionStateEnum::JUMP) {
			return true;
		}
		break;

	case ECharacterActionStateEnum::CROUCH:
		if (updatedAction != ECharacterActionStateEnum::JUMP) {
			return true;
		}
		break;

	case ECharacterActionStateEnum::PICKING:
		if (updatedAction != ECharacterActionStateEnum::PULLING) {
			return true;
		}
		break;

	case ECharacterActionStateEnum::PULLING:
		return false;
		break;
	}

	return false;
}

void APlayerCharacter::UpdateActionState(ECharacterActionStateEnum newAction)
{
	if (newAction == ECharacterActionStateEnum::MOVE
		|| newAction == ECharacterActionStateEnum::IDLE)
	{
		if (CharacterActionState == ECharacterActionStateEnum::JUMP) return;
		if (FMath::Abs(GetVelocity().Size()) <= 0.01f)
		{
			CharacterActionState = ECharacterActionStateEnum::IDLE;
		}
		else
		{
			CharacterActionState = ECharacterActionStateEnum::MOVE;
		}
	}
	else
	{
		CharacterActionState = newAction;
	}
}

void APlayerCharacter::JumpAction() {

}

void APlayerCharacter::FinalizeJump() {
	CharacterActionState = ECharacterActionStateEnum::IDLE;
}

void APlayerCharacter::DoMoveForward(float scale)
{
	AddMovementInput(GetActorForwardVector(), scale);
}

void APlayerCharacter::DoMoveRight(float scale)
{
	AddMovementInput(GetActorRightVector(), scale);
}

void APlayerCharacter::PerformInteraction()
{
	UE_LOG(LogTemp, Log, TEXT("Interaction Starting"));

	TArray<AActor*> OverlappingActors;
	this->GetOverlappingActors(OverlappingActors);
	for (AActor* actor : OverlappingActors) {
	}
}

void APlayerCharacter::FinalizeInteraction()
{
	if (CharacterActionState != ECharacterActionStateEnum::PULLING) {
		InteractEnd();
	}
}

void APlayerCharacter::InteractEnd()
{
	UE_LOG(LogTemp, Log, TEXT("Interaction Ending"));
	UpdateActionState(ECharacterActionStateEnum::IDLE);
}

void APlayerCharacter::RotateSpringArm(float pitch, float yaw, float roll)
{
	FRotator rot = SpringArm->GetRelativeRotation();
	rot += FRotator(pitch, yaw, roll);
	if (rot.Pitch < SpringArmMaxPitch && rot.Pitch > -SpringArmMaxPitch) {
		SpringArm->SetRelativeRotation(rot);
	}
}

void APlayerCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult) {

}

void APlayerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	Super::FellOutOfWorld(dmgType);
	UGameplayStatics::OpenLevel(this, FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));

	// GetWorld()->ServerTravel(FString("/Game/Maps/").Append(UGameplayStatics::GetCurrentLevelName(GetWorld()).Append("?listen")));
}

void APlayerCharacter::StoreBlock(int32 block) {
	++Inventory[block];
}

bool APlayerCharacter::UseBlock(int32 block) {
	if(Inventory[block] > 0){
		--Inventory[block];
		return true;
	}
	return false;
}

TArray<int32> APlayerCharacter::GetInventory() {
	return Inventory;
}