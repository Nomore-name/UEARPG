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
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class USActionComponent;
class USUserWidgetEnemyHealthBar;

UCLASS()
class UNREALARPG_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	enum 
	{
		E_SKILL_TRANS		= 1,
		E_SKILL_FIRE_BUFF	= 2,
		E_SKILL_BLACK_HOLE	= 3,
	};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//移动
	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);
	void Run(const FInputActionValue& InputValue);
	void RunRelease(const FInputActionValue& InputValue);
	void Defense(const FInputActionValue& InputValue);
	void DefenseRelease(const FInputActionValue& InputValue);
	//技能
	void ReleaseSkill(const FInputActionInstance& Instance);
	//void SkillTransfer_TimeEnd();
	//普攻
	void PrimaryAttack();
	void ReloadPrimaryAttack(const FInputActionValue& InputValue);
	UFUNCTION(Server, Reliable)
	void ServerReloadPrimaryAttack(USProjectileAction* PrimaryAttackAction);
	//void PrimaryAttackByPlayer();
	//void PrimaryAttack_TimeEnd();
	//生成粒子
	//void ReleaseProjectileByCamera(TSubclassOf<AActor>& Projectile);
	//绘制方向线
	void DrwaDebugArrows();
	//交互
	void PrimaryInteract();
	//生命值改变
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float ChangeValue);
	UFUNCTION()
	void OnShieldHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float ChangeValue);
	//让角色朝向转为控制器朝向
	void SetActorRotationFromController();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	const UCameraComponent* GetPlayerCamera() const;

	UFUNCTION()
	UChildActorComponent* GetChildDefenseComp() const;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

	UFUNCTION(Exec)
	void SetLife(float life);

	//游戏暂停
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void TogglePauseMenu();

protected:
	//输入移动相关
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* m_PlayerInputMap = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputMove = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputLookMouse = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputPrimaryAttack = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputJump = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputInteract = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputSkill = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputRun = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputDef = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputReload= nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* m_InputPause= nullptr;

	//相机相关
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* m_SpringArmComp = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* m_CameraComp = nullptr;

	//交互相关
	UPROPERTY(VisibleAnywhere, Category = "Interact")
	USInteractionComponent *m_InteractionComp = nullptr;

	//属性相关
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	USAttributeComponent* m_AttributeComp = nullptr;

	//行为相关
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	USActionComponent* m_ActionComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shield")
	UChildActorComponent* m_ChildDefenseComp = nullptr;

	//血条UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USUserWidgetEnemyHealthBar> m_HealthBarClass;

	UPROPERTY()
	USUserWidgetEnemyHealthBar* m_HealthBar = nullptr;

	//攻击相关(已弃用，改用Action实现）
	/*
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AActor> m_ProjectleClass = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AActor> m_ProTransClass = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	UAnimMontage* m_AttackAnim = nullptr;
private:
	FTimerHandle m_TimeHandlePrimaryAttack;
	FTimerHandle m_TimeHandleTransferSkill;
	*/
};
