// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
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
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

	//Establishes the variable for health and can be changed within the blueprint class.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;

	//Establishes the variable for hunger and can be changed within the blueprint class.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;

	//Establishes the variable for stamina and can be changed within the blueprint class.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;

	//Sets up an integer for Wood
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Wood;

	//Sets up an integer for Stone
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Stone;

	//Sets up an integer for Berries
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Berry;

	//Sets up the number of resources in an array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	//Sets up the resources names in an array where the first array is wood followed by stone then berries
	UPROPERTY(EditAnywhere, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	//Sets a custom event for health that starts decreasing when hunger is 0.
	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount);

	//Sets a custom event for hunger that starts decreasing health when it hits 0.
	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	//Sets a custom event for stamina
	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	//Sets a custom event for a timer that starts decreasing stats over time.
	UFUNCTION()
		void DecreaseStats();

	//Sets a custom event for getting resources
	UFUNCTION()
		void GiveResource(float amount, FString resourceType);


};
