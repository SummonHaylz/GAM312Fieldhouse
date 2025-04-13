// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_FIELDHOUSE_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

	//Sets the functions for UpatematOBJ and updatebuildObj to public so they can be called anywhere. These will update both objectives.
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdatematOBJ(float matsCollected);

	UFUNCTION(BlueprintImplementableEvent)
		void updatebuildObj(float objectsBuilt);
};
