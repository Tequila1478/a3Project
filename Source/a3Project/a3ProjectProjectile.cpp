// Copyright Epic Games, Inc. All Rights Reserved.

#include "a3ProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

struct FOnHitParams
{
	float Damage;
	AActor* Object;
};

Aa3ProjectProjectile::Aa3ProjectProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &Aa3ProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void Aa3ProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Hit"));

    if (OtherActor != nullptr)
    {
        static UClass* InteractInterfaceClass = LoadObject<UClass>(nullptr, TEXT("/Game/BP/I_Interact.I_Interact_C"));

        if (!InteractInterfaceClass)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load interface class!"));
            return;
        }

        if (OtherActor->GetClass()->ImplementsInterface(InteractInterfaceClass))
        {
            UE_LOG(LogTemp, Log, TEXT("Actor implements I_Interact"));

            FName FunctionName = FName("OnHit");
            UFunction* Function = OtherActor->FindFunction(FunctionName);

            if (Function)
            {
                UE_LOG(LogTemp, Log, TEXT("Found OnHit function. Calling it now."));

                // Prepare parameters
                struct FOnHitParams
                {
                    float Damage;
                    AActor* Object;
                };

                FOnHitParams Params;
                Params.Damage = 10.0f;       // The float value you want to send
                Params.Object = this;       // Pass this projectile as reference

                OtherActor->ProcessEvent(Function, &Params);
				this->Destroy(); // Destroy the projectile after processing the event
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("OnHit function NOT found on actor."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor does NOT implement I_Interact interface."));
        }
    }
}