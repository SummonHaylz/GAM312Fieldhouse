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

	//Establishes the building array
	BuildingArray.SetNum(3);
	//Establishes the resource arrays element is set to 3 since there are 3 resources
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));
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

	playerUI->UpdateBars(Health, Hunger, Stamina);

	//Sets conditions on if building
	if (!isBuilding)
	{
		//Sets conditions on if a part is spawned and allows the player to place the part and follows the camera
		if (spawnedPart)
		{
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation * Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}
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

	//Rotates buildings
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);
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
	//Establishes a hit result local variable
	FHitResult HitResult;

	//Grabs the start location which is the camera component location
	FVector StartLocation = PlayerCamComp->GetComponentLocation();

	//Sets the direction which is the forward vector of the camera 800 units ahead
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;

	//End location adds the start location to the direction
	FVector EndLocation = StartLocation + Direction;

	//sets the collision query parameters
	FCollisionQueryParams QueryParams;

	//ignores the player character
	QueryParams.AddIgnoredActor(this);

	//Allows us to see complex traces
	QueryParams.bTraceComplex = true;

	//Returns the face to normal
	QueryParams.bReturnFaceIndex = true;

	//Sets if building is not true then line tracing can continue
	if (!isBuilding)
	{
		//Performs the line tracing
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			//Casts to the resource
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			//Ensures the editor does not crash if a resource is not hit and runs if stamina is greater than 5
			if (Stamina > 5.0f)
			{
				//Gets the hit name of the resource that was collected
				FString hitName = HitResource->resourceName;

				//Gets the resource value and checks for the resource amount in the hit resource
				int resourceValue = HitResource->resourceAmount;

				//Calculates the new resource value
				HitResource->totalResource = HitResource->totalResource - resourceValue;

				//Only runs if there is a total resouce left
				if (HitResource->totalResource > resourceValue)
				{
					//Lists the resource value and hit name in Fstring
					GiveResource(resourceValue, hitName);

					//Checks to see if the player is hitting
					check(GEngine != nullptr)
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

					//Calls the decal at location and specifies the size and rotation as well as lifespan
					UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

					//Sets the new stamina to have 5 less
					SetStamina(-5.0f);
				}
				//Destroys the resource if depleted
				else
				{
					HitResource->Destroy();
					check(GEngine != nullptr)
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
				}
			}
		}
	}
	//Stops the tick event
	else
	{
		isBuilding = false;
		
	}
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

void APlayerChar::GiveResource(float amount, FString resourceType)
{
	//Sets up the array for the resources and adds to the amount
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{	
	if (woodAmount <= ResourcesArray[0])
	{
		//Subtracts the resources in order to build the object
		ResourcesArray[0] = ResourcesArray[0] - woodAmount;
		ResourcesArray[1] = ResourcesArray[1] - stoneAmount;
		//Adds one to the building object dependent on the type
		if (buildingObject == "Wall")
		{
			BuildingArray[0] = BuildingArray[0] + 1;
		}
		if (buildingObject == "Floor")
		{
			BuildingArray[1] = BuildingArray[1] + 1;
		}
		if (buildingObject == "Ceiling")
		{
			BuildingArray[2] = BuildingArray[2] + 1;
		}
	}
}

void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	//Executes if not building
	if (!isBuilding)
	{
		//Checks to see if the building array has a particular object
		if (BuildingArray[buildingID] >= 1)
		{
			//Sets that the player is building
			isBuilding = true;

			//Sets spawn location
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			//Subtracts 1 from the building array for the spawned part
			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			//Spawns in the part from TSubclassOf<ABuildingPart> and sets it to the children of the class as well.
			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			//Shows if the actions were successful
			isSuccess = true;
		}
		//Shows if the actions were not successful
		isSuccess = false;
	}
}

void APlayerChar::RotateBuilding()
{
	//Sets the rotation
	spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
}


