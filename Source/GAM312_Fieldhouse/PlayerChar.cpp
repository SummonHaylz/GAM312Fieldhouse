// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initial setup of camera component.
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	//Attaching camera to the character mesh and head bone.
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	//Share rotation with controller.
	PlayerCamComp->bUsePawnControlRotation = true;


}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	//Sets a timer that calls on the DecreaseStats function that goes into effect every two seconds and affects the player character.
	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Binds the axis to move forward 
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	
	//Binds the axis to move right
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);

	//Binds the axis to look up
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);

	//Binds the axis to turn
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);

	//Binds the action to jump if the spacebar is pressed
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);

	//Binds the action to stop jumping if the spacebar is released
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

	//Binds the action to interact if LMB is clicked
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
}

void APlayerChar::MoveForward(float axisValue)
{
	//Describes the function of moving forward
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::MoveRight(float axisValue)
{
	//Describes the function of moving right
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::StartJump()
{
	//Describes the function of StartJump
	bPressedJump = true;
}

void APlayerChar::StopJump()
{
	//Describes the function of StopJump
	bPressedJump = false;
}

void APlayerChar::FindObject()
{
}

void APlayerChar::SetHealth(float amount)
{
	//ensures health does not exceed 100.
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}

void APlayerChar::SetHunger(float amount)
{
	//ensures hunger does not exceed 100
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}

void APlayerChar::SetStamina(float amount)
{
	//ensures stamina does not exceed 100
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
}

void APlayerChar::DecreaseStats()
{
	//Rules on if hunger is greater than 0
	if (Hunger > 0)
	{
		//subtracts 1 from hunger every time the interval is called.
		SetHunger(-1.0f);
	}
	
	//Sets stamina regeneration
	SetStamina(10.0f);

	//if hunger is less than or equal to 0, health will start decreasing
	if (Hunger <= 0)
	{
		//Affects how much health is taken
		SetHealth(-3.0f);
	}
}


