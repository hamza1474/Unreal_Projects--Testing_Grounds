// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons_Rifle.h"
#include "BallProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

// Sets default values
AWeapons_Rifle::AWeapons_Rifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	// only the owning player will see this mesh
	Gun->bCastDynamicShadow = false;
	Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	Gun->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(Gun);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

}

// Called when the game starts or when spawned
void AWeapons_Rifle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapons_Rifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapons_Rifle::OnFire()
{
	UE_LOG(LogTemp, Warning, TEXT("FPP FIRE METHOD"))
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (!MuzzleLocation) { return; }
			const FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation =  MuzzleLocation->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
