// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include <Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "Components/SAttributeComponent.h"
#include "Components/SInteractionComponent.h"
#include <Action/SActionComponent.h>
#include <Runtime/Engine/Classes/Components/ChildActorComponent.h>
#include <Character/SShield.h>
#include "Action/SProjectileAction.h"
#include <../UnrealARPG.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <UI/SUserWidgetEnemyHealthBar.h>
#include "Components/SProjectilePoolComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	m_SpringArmComp->SetupAttachment(RootComponent);
	m_SpringArmComp->bUsePawnControlRotation = true;	 //使相机跟随着角色旋转

	m_CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	m_CameraComp->SetupAttachment(m_SpringArmComp);

	m_InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	m_AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	m_ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	m_ChildDefenseComp = CreateDefaultSubobject<UChildActorComponent>("ChildComp");
	m_ChildDefenseComp->SetupAttachment(GetMesh(), TEXT("shield"));
	m_ChildDefenseComp->SetChildActorClass(ASShield::StaticClass());

	m_ProjectilePoolComp = CreateDefaultSubobject<USProjectilePoolComponent>("ProjectileComp");
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 需放在BeginPlay中赋值，否则派生出的角色蓝图类可能覆盖掉该值！！
	
	//使角色朝向跟随移动方向
	//GetCharacterMovement()->bOrientRotationToMovement = true;

	//如果Controller的朝向和角色朝向不一致，角色朝向就会平滑的按照RotationRate里配置的各个轴的旋转速率来旋转角色的Rotation到Controller的朝向。
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//bUseControllerRotationYaw为true时，角色的Rotation会与Controller的Rotation始终一致，该选项与bUseControllerDesiredRotation通常互斥。
	bUseControllerRotationYaw = false;

	//默认角色不开启护盾
	if (m_ChildDefenseComp) {
		m_ChildDefenseComp->Deactivate();
		m_ChildDefenseComp->SetVisibility(false);
	}
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

	if (!m_PlayerInputMap) {
		return;
	}

	const APlayerController* PlayerControler = GetController<APlayerController>();
	const ULocalPlayer* LocalPlayer = PlayerControler->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EInputSubsys = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (EInputSubsys) {
		EInputSubsys->ClearAllMappings();
		EInputSubsys->AddMappingContext(m_PlayerInputMap, 0);
	}

	UEnhancedInputComponent* EInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EInputComp) {
		EInputComp->BindAction(m_InputMove, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EInputComp->BindAction(m_InputLookMouse, ETriggerEvent::Triggered, this, &ASCharacter::LookMouse);
		EInputComp->BindAction(m_InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EInputComp->BindAction(m_InputPrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
		EInputComp->BindAction(m_InputInteract, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
		EInputComp->BindAction(m_InputSkill, ETriggerEvent::Triggered, this, &ASCharacter::ReleaseSkill);
		EInputComp->BindAction(m_InputRun, ETriggerEvent::Started, this, &ASCharacter::Run);
		EInputComp->BindAction(m_InputRun, ETriggerEvent::Completed, this, &ASCharacter::RunRelease);
		EInputComp->BindAction(m_InputDef, ETriggerEvent::Started, this, &ASCharacter::Defense);
		EInputComp->BindAction(m_InputDef, ETriggerEvent::Completed, this, &ASCharacter::DefenseRelease);
		EInputComp->BindAction(m_InputReload, ETriggerEvent::Triggered, this, &ASCharacter::ReloadPrimaryAttack);
		EInputComp->BindAction(m_InputPause, ETriggerEvent::Triggered, this, &ASCharacter::TogglePauseMenu);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (m_AttributeComp) {
		m_AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	}

	if (m_ChildDefenseComp) {
		ASShield * Shield = Cast<ASShield>(m_ChildDefenseComp->GetChildActor());

		if (Shield) {
			Shield->GetAttributeComp()->OnHealthChanged.AddDynamic(this, &ASCharacter::OnShieldHealthChanged);
		}
	}
}

 const UCameraComponent* ASCharacter::GetPlayerCamera() const
{
	return m_CameraComp;
}

UChildActorComponent* ASCharacter::GetChildDefenseComp() const
 {
	return m_ChildDefenseComp;
 }

void ASCharacter::HealSelf(float Amount /*= 100.f*/)
{
	if (m_AttributeComp) {
		m_AttributeComp->AddHealth(this, Amount);
	}
}

void ASCharacter::SetLife(float life)
{
	if (m_AttributeComp) {
		float curLife = m_AttributeComp->GetCurHealth();
		m_AttributeComp->AddHealth(this, life - curLife);
	}
}

void ASCharacter::SpawnProjectile(FTransform Transform)
{
	if (m_ProjectilePoolComp) {
		m_ProjectilePoolComp->SpawnFromPool(Transform);
	}
}

void ASCharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRotator = GetControlRotation();
	ControlRotator.Pitch = 0.f;
	ControlRotator.Roll = 0.f;

	const FVector2D AxisVal = Instance.GetValue().Get<FVector2D>();
	AddMovementInput(ControlRotator.Vector(), AxisVal.Y);

	//获取当前控制旋转的Y轴方向向量(根据玩家的视角方向确定移动方向)
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

void ASCharacter::Run(const FInputActionValue& InputValue)
{
	if (m_ActionComp) {
		m_ActionComp->StartActionByName(this, "Run");
	}
}

void ASCharacter::RunRelease(const FInputActionValue& InputValue)
{
	if (m_ActionComp) {
		m_ActionComp->StopActionByName(this, "Run");
	}
}

void ASCharacter::Defense(const FInputActionValue& InputValue)
{
	if (m_ChildDefenseComp) {
		ASShield* Shield = Cast<ASShield>(m_ChildDefenseComp->GetChildActor());

		if (Shield && Shield->GetAttributeComp()->GetCurHealth() <= 0) {
			return;
		}
	}

	if (m_ActionComp) {
		StopAnimMontage(GetCurrentMontage());
		m_ActionComp->StartActionByName(this, "Defense");
	}
}

void ASCharacter::DefenseRelease(const FInputActionValue& InputValue)
{
	if (m_ActionComp) {
		m_ActionComp->StopActionByName(this, "Defense");
	}
}

void ASCharacter::DrwaDebugArrows()
{
	if (!CVarDrawDebugLine.GetValueOnGameThread()) {
		return;
	}

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

void ASCharacter::ReleaseSkill(const FInputActionInstance& Instance)
{
	SetActorRotationFromController();

	FVector Value = Instance.GetValue().Get<FVector>();
	//UE_LOG(LogTemp, Log, TEXT("ReleaseSkill: %s"), *Value.ToString());

	if (!m_ActionComp) {
		return;
	}

	switch (static_cast<int>(Value.X))
	{
	case E_SKILL_TRANS:
		m_ActionComp->StartActionByName(this, "SkillTrans");
		break;
	case E_SKILL_FIRE_BUFF:
		m_ActionComp->StartActionByName(this, "FireBuff");
		break;
	case E_SKILL_BLACK_HOLE:
		m_ActionComp->StartActionByName(this, "SkillBlackHole");
		break;
	default:
		break;
	}
}

void ASCharacter::PrimaryAttack()
{
	SetActorRotationFromController();		//防止出现攻击时角色朝向与控制器朝向不一致的问题
	if (m_ActionComp) {
		m_ActionComp->StartActionByName(this, "PrimaryAttack");
	}

	/*PlayAnimMontage(m_AttackAnim);
	GetWorldTimerManager().SetTimer(m_TimeHandlePrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeEnd, 0.2f);*/
}

void ASCharacter::ReloadPrimaryAttack(const FInputActionValue& InputValue)
{
	if (!m_ActionComp) {
		return;
	}

	USProjectileAction* PrimaryAttackAction = Cast<USProjectileAction>(m_ActionComp->GetAction("PrimaryAttack"));
	if (!PrimaryAttackAction) {
		return;
	}

	if (PrimaryAttackAction->CanStart(this)) {
		PrimaryAttackAction->SetIsReload(true);
		PrimaryAttackAction->ReloadProjectile(this);
	}

	if (!HasAuthority()) {
		ServerReloadPrimaryAttack(PrimaryAttackAction);
	}
}

void ASCharacter::ServerReloadPrimaryAttack_Implementation(USProjectileAction* PrimaryAttackAction)
{
	if (!PrimaryAttackAction) {
		return;
	}

	if (PrimaryAttackAction->CanStart(this)) {
		PrimaryAttackAction->SetIsReload(true);
		PrimaryAttackAction->ReloadProjectile(this);
	}
}

void ASCharacter::PrimaryInteract()
{
	SetActorRotationFromController();
	if (m_InteractionComp) {
		m_InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float ChangeValue)
{
	if (!IsLocallyControlled() && !m_HealthBar && NewHealth > 0) {
		m_HealthBar = CreateWidget<USUserWidgetEnemyHealthBar>(GetWorld(), m_HealthBarClass);
		m_HealthBar->m_AttachedActor = this;
        m_HealthBar->AddToViewport();
	}

	if (NewHealth <= 0) {
		APlayerController* PlayerControl = Cast<APlayerController>(GetController());
		DisableInput(PlayerControl);
	}
}

void ASCharacter::OnShieldHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float ChangeValue)
{
	if (NewHealth <= 0 && m_ActionComp && m_ActionComp->IsAciveTag(FGameplayTag::RequestGameplayTag(FName("Action.Defending")))) {
		m_ActionComp->StopActionByName(this, "Defense");
	}
}

void ASCharacter::SetActorRotationFromController()
{
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardVector = ControlRotation.Vector();
	SetActorRotation(ForwardVector.Rotation());
}

//原始攻击逻辑（弃用，改用Action实现）
/*
void ASCharacter::PrimaryAttack_TimeEnd()
{
	ReleaseProjectileByCamera(m_ProjectleClass);
	//PrimaryAttackByPlayer();
}

void ASCharacter::SkillTransfer_TimeEnd()
{
	ReleaseProjectileByCamera(m_ProTransClass);
}

void ASCharacter::ReleaseProjectileByCamera(TSubclassOf<AActor>& Projectile)
{
	FVector CameraSeeEnd = m_CameraComp->GetComponentLocation() + (m_CameraComp->GetComponentRotation().Vector() * 10000);
	TArray<FHitResult> HitsArray;
	FCollisionShape CollisionShape;
	FCollisionObjectQueryParams CollisionParams;

	CollisionShape.SetSphere(10.f);
	CollisionParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionParams.AddObjectTypesToQuery(ECC_WorldStatic);

	bool bHit = GetWorld()->SweepMultiByObjectType(HitsArray, m_CameraComp->GetComponentLocation(), CameraSeeEnd, FQuat::Identity, CollisionParams, CollisionShape);

	FVector HitLocation = bHit ? HitsArray[0].ImpactPoint : CameraSeeEnd;
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator HitRotator = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitLocation);
	FTransform SpawnTrans = FTransform(HitRotator, HandLocation);

	if (bHit) {
		UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *HitLocation.ToString());
	} else {
		UE_LOG(LogTemp, Log, TEXT("No Hit: %s"), *HitLocation.ToString());
	}

	//配置在生成新的Actor时的各种参数
	FActorSpawnParameters SpawnParams;
	//SpawnCollisionHandlingOverride用于指定引擎在生成新Actor时应该如何处理碰撞,AlwaysSpawn表示总生成新的Actor,即使它可能会与现有的Actor发生重叠
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(Projectile, SpawnTrans, SpawnParams);

	if (CVarDrawDebugLine.GetValueOnGameThread()) {
		FColor LineColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), m_CameraComp->GetComponentLocation(), HitLocation, LineColor, false, 2.0f, 0, 2.0f);
	}
}

void ASCharacter::PrimaryAttackByPlayer()
{
	FVector HandLocal = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTrans = FTransform(GetControlRotation(), HandLocal);

	//配置在生成新的Actor时的各种参数
	FActorSpawnParameters SpawnParams;
	//SpawnCollisionHandlingOverride用于指定引擎在生成新Actor时应该如何处理碰撞,AlwaysSpawn表示总生成新的Actor,即使它可能会与现有的Actor发生重叠
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(m_ProjectleClass, SpawnTrans, SpawnParams);

	if (CVarDrawDebugLine.GetValueOnGameThread()) {
		FVector ControllerDirection_LineEnd = HandLocal + (GetControlRotation().Vector() * 1000.0f);
		DrawDebugDirectionalArrow(GetWorld(), HandLocal, ControllerDirection_LineEnd, 100.0f, FColor::Red, false, 2.0f, 0, 5.0f);
	}
}
*/
