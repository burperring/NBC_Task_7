// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameMode.h"
#include "HeliPlayerController.h"

ATaskGameMode::ATaskGameMode()
{
	PlayerControllerClass = AHeliPlayerController::StaticClass();
}
