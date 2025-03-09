// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_FIELDHOUSE_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Sets up the function for move forward
	UFUNCTION()
		void MoveForward(float axisValue);

	//Sets up the function for move right
	UFUNCTION()
		void MoveRight(float axisValue);

	//Sets up the function to start jumping
	UFUNCTION()
		void StartJump();

	//Sets up the function to stop jumping
	UFUNCTION()
		void StopJump();

	//Sets up the function to find objects
	UFUNCTION()
		void FindObject();
	
	//Sets the camera component to the player
	UPROPERTY()
	UCameraComponent* PlayerCamComp;


};
