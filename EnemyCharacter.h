// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class SACREDHEART_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, Category = "Enemy Behaviour")
	class UBehaviorTree *BotsBehaviour;

	/** The speed at which the character will be walking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		float WalkSpeed;

	/** The speed at which the character will be sprinting*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		float SprintSpeed;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
