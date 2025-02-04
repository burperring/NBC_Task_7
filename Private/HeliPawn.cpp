// Fill out your copyright notice in the Description page of Project Settings.


#include "HeliPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "EnhancedInputComponent.h"
#include "HeliPlayerController.h"

// Sets default values
AHeliPawn::AHeliPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(1);
	Mesh->SetAngularDamping(1);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Mesh);
	SpringArm->TargetArmLength = 800;
	SpringArm->SetRelativeRotation(FRotator(-20, 0, 0));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	PhysicsThrus = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("Physics Thruster"));
	PhysicsThrus->SetupAttachment(Mesh);
	PhysicsThrus->SetRelativeRotation(FRotator(-90, 0, 0));
	PhysicsThrus->bAutoActivate = true;

	this->AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AHeliPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FVector ThrusterCenterOfMassDifference = PhysicsThrus->GetComponentLocation() - Mesh->GetCenterOfMass();
	FRotator InvertedMeshRotation = Mesh->GetComponentRotation().GetInverse();
	FVector CenterOfMassActualOffset = InvertedMeshRotation.RotateVector(ThrusterCenterOfMassDifference);
	FVector CenterOfMassWantedOffset = FVector(CenterOfMassActualOffset.X, CenterOfMassActualOffset.Y, 0);
	Mesh->SetCenterOfMass(CenterOfMassWantedOffset, FName());
}

// Called every frame
void AHeliPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHeliPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (AHeliPlayerController* PlayerController = Cast<AHeliPlayerController>(GetController())) {
			if (PlayerController->MoveAction) {
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AHeliPawn::Move
				);

				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::None,
					this,
					&AHeliPawn::Move
				);
			}

			if (PlayerController->LookAction) {
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AHeliPawn::Look
				);
			}

			if (PlayerController->RotateAction) {
				EnhancedInput->BindAction(
					PlayerController->RotateAction,
					ETriggerEvent::Triggered,
					this,
					&AHeliPawn::Rotate
				);

				EnhancedInput->BindAction(
					PlayerController->RotateAction,
					ETriggerEvent::None,
					this,
					&AHeliPawn::Rotate
				);
			}

			if (PlayerController->UpDownActioin) {
				EnhancedInput->BindAction(
					PlayerController->UpDownActioin,
					ETriggerEvent::Triggered,
					this,
					&AHeliPawn::UpDownStart
				);

				EnhancedInput->BindAction(
					PlayerController->UpDownActioin,
					ETriggerEvent::Completed,
					this,
					&AHeliPawn::UpDownEnd
				);
			}
		}
	}
}

void AHeliPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	float DesiredAngleX = MoveInput.X * DesiredTiltAngle + Mesh->GetComponentRotation().Roll;
	float ClampedValueX = FMath::Clamp(DesiredAngleX, -TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorForwardVector() * ClampedValueX * TiltingSpeed, FName(), true);

	float DesiredAngleY = MoveInput.Y * DesiredTiltAngle + Mesh->GetComponentRotation().Pitch;
	float ClampedValueY = FMath::Clamp(DesiredAngleY, -TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorRightVector() * ClampedValueY * TiltingSpeed, FName(), true);
}

void AHeliPawn::Look(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D LookInput = value.Get<FVector2D>();

	SpringArm->AddRelativeRotation(FRotator(LookInput.Y, LookInput.X * -1, 0));
}

void AHeliPawn::Rotate(const FInputActionValue& value)
{
	if (!Controller) return;

	const float RotateInput = value.Get<float>();

	Mesh->AddTorqueInDegrees(GetActorUpVector() * RotateInput * YawRotationSpeed, FName(), true);
}

void AHeliPawn::UpDownStart(const FInputActionValue& value)
{
	if (!Controller) return;

	const float UpDown = value.Get<float>();

	float DesiredUpForce = UpDown * VariableUpForce + ConstantUpForce;
	PhysicsThrus->ThrustStrength = DesiredUpForce / GetActorUpVector().Z * Mesh->GetMass();
}

void AHeliPawn::UpDownEnd(const FInputActionValue& value)
{
	if (!Controller) return;

	PhysicsThrus->ThrustStrength = ConstantUpForce / GetActorUpVector().Z * Mesh->GetMass();
}

void AHeliPawn::SetDesiredTiltAngle(float Value)
{
	DesiredTiltAngle = FMath::Clamp(Value, -60.0f, 60.0f);
}

float AHeliPawn::GetDesiredTiltAngle()
{
	return DesiredTiltAngle;
}

void AHeliPawn::SetTiltSpeedClampBound(float Value)
{
	TiltSpeedClampBound = Value > 0 ? Value : 0;
}

float AHeliPawn::GetTiltSpeedClampBound()
{
	return TiltSpeedClampBound;
}

void AHeliPawn::SetTiltingSpeed(float Value)
{
	TiltingSpeed = Value > 0 ? Value : 0;
}

float AHeliPawn::GetTiltingSpeed()
{
	return TiltingSpeed;
}
