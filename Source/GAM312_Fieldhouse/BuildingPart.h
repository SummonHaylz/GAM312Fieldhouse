// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "BuildingPart.generated.h"

UCLASS()
class GAM312_FIELDHOUSE_API ABuildingPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Attaches the mesh
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	//Attaches the arrow component will give a pivot location when the model is made (start location)
	UPROPERTY(EditAnywhere)
		UArrowComponent* PivotArrow;
};
