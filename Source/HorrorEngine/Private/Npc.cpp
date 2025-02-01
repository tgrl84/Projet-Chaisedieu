#include "Npc.h"

// Sets default values
ANpc::ANpc()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentPointIndex = 0;
	bLoop = false;
	MovementSpeed = 200.0f; // Default speed
	bStart = false;
}

// Called when the game starts or when spawned
void ANpc::BeginPlay()
{
	Super::BeginPlay();
	
	if (Points.Num() > 0)
	{
		MoveToNextPoint();
	}
}

// Called every frame
void ANpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Points.Num() > 0 && bStart)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = Points[CurrentPointIndex];
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, TargetLocation) < 10.0f) // Threshold to consider as reached
		{
			MoveToNextPoint();
		}
	}
}

void ANpc::MoveAlongPoints()
{
	if (Points.Num() > 0)
	{
		CurrentPointIndex = 0;
		MoveToNextPoint();
	}
}

void ANpc::MoveToNextPoint()
{
	if (Points.Num() == 0)
	{
		return;
	}

	CurrentPointIndex++;
	if (CurrentPointIndex >= Points.Num())
	{
		if (bLoop)
		{
			CurrentPointIndex = 0;
		}
		else
		{
			Points.Empty();
		}
	}
}

void ANpc::StartMovement()
{
	bStart = true;
}



