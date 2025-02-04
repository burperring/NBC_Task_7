// Fill out your copyright notice in the Description page of Project Settings.


#include "HeliPlayerController.h"
#include "EnhancedInputSubsystems.h"

AHeliPlayerController::AHeliPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	RotateAction(nullptr),
	UpDownActioin(nullptr)
{
}

void AHeliPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get Local Player
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) {
		// Get Local Player EnhancedInputLocalPlayerSubsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			if (InputMappingContext) {
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
