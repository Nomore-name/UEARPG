// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "SCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
struct FInputActionInstance;
struct FInputActionValue;
UCLASS()
class UNREALARPG_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);
	void DrwaDebugArrows();
	void PrimaryAttack();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* m_SpringArmComp = nullptr;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_CameraComp = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* m_PlayerInputMap = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputMove = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputLookMouse = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputPrimaryAttack = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> m_ProjectleClass;
};
