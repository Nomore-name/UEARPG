// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include <Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	m_SpringArmComp->SetupAttachment(RootComponent);

	m_CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	m_CameraComp->SetupAttachment(m_SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	// ʹ��ɫ�ӽ����ſ������䶯
	// �����BeginPlay�и�ֵ�������������Ľ�ɫ��ͼ����ܸ��ǵ���ֵ����
	//bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	//ʹ��������Ž�ɫ��ת
	m_SpringArmComp->bUsePawnControlRotation = true;
	
	// ʹ��ɫ��������ƶ�����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRotator = GetControlRotation();
	ControlRotator.Pitch = 0.f;
	ControlRotator.Roll = 0.f;

	const FVector2D AxisVal = Instance.GetValue().Get<FVector2D>();
	AddMovementInput(ControlRotator.Vector(), AxisVal.Y);

	//��ȡ��ǰ������ת��Y�᷽������(������ҵ��ӽǷ���ȷ���ƶ�����)
	const FVector CurDirVector = FRotationMatrix(ControlRotator).GetScaledAxis(EAxis::Y);
	AddMovementInput(CurDirVector, AxisVal.X);
}

void ASCharacter::LookMouse(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	//UE_LOG(LogTemp, Log, TEXT("%s"), *Value.ToString());

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ASCharacter::DrwaDebugArrows()
{
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

void ASCharacter::PrimaryAttack()
{
	FVector HandLocal = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTrans = FTransform(GetControlRotation(), HandLocal);

	//�����������µ�Actorʱ�ĸ��ֲ���
	FActorSpawnParameters SpawnParams;
	//SpawnCollisionHandlingOverride����ָ��������������ActorʱӦ����δ�����ײ,AlwaysSpawn��ʾ�������µ�Actor,��ʹ�����ܻ������е�Actor�����ص�
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(m_ProjectleClass, SpawnTrans, SpawnParams);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrwaDebugArrows();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!m_InputMove || !m_PlayerInputMap) {
		return;
	}

	const APlayerController* PlayerControler = GetController<APlayerController>();
	const ULocalPlayer* LocalPlayer = PlayerControler->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EInputSubsys = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	UEnhancedInputComponent* EInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EInputSubsys) {
		EInputSubsys->ClearAllMappings();
		EInputSubsys->AddMappingContext(m_PlayerInputMap, 0);
	}

	if (EInputComp) {
		EInputComp->BindAction(m_InputMove, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EInputComp->BindAction(m_InputLookMouse, ETriggerEvent::Triggered, this, &ASCharacter::LookMouse);
		EInputComp->BindAction(m_InputPrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
	}
}
