// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HeliPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPhysicsThrusterComponent;
struct FInputActionValue;

UCLASS()
class NBC_TASK_7_API AHeliPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHeliPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heli")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heli")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heli")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heli")
	UPhysicsThrusterComponent* PhysicsThrus;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Rotate(const FInputActionValue& value);
	UFUNCTION()
	void UpDownStart(const FInputActionValue& value);
	UFUNCTION()
	void UpDownEnd(const FInputActionValue& value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heli Movement")
	float VariableUpForce = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heli Movement")
	float ConstantUpForce = 970;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heli Movement")
	float YawRotationSpeed = 60;

	UFUNCTION(BlueprintSetter)
	void SetDesiredTiltAngle(float Value);
	UFUNCTION(BlueprintGetter)
	float GetDesiredTiltAngle();
	UFUNCTION(BlueprintSetter)
	void SetTiltSpeedClampBound(float Value);
	UFUNCTION(BlueprintGetter)
	float GetTiltSpeedClampBound();
	UFUNCTION(BlueprintSetter)
	void SetTiltingSpeed(float Value);
	UFUNCTION(BlueprintGetter)
	float GetTiltingSpeed();

private:
	UPROPERTY(EditAnywhere, BlueprintSetter = SetDesiredTiltAngle, BlueprintGetter = GetDesiredTiltAngle,
				Category = "Heli Movement", meta = (ClampMin = -60, ClampMax = 60))
	float DesiredTiltAngle = 30;
	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltSpeedClampBound, BlueprintGetter = GetTiltSpeedClampBound,
				Category = "Heli Movement", meta = (ClampMin = 0))
	float TiltSpeedClampBound = 20;
	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltingSpeed, BlueprintGetter = GetTiltingSpeed,
				Category = "Heli Movement", meta = (ClampMin = 0))
	float TiltingSpeed = 5;
};
