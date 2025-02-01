#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Npc.generated.h"

UCLASS()
class HORRORENGINE_API ANpc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANpc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Array of points in space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<FVector> Points;

	// Boolean to determine if looping is enabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bLoop;

	// Speed of movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed;

	// Function to move the actor
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveAlongPoints();

	// Boolean to determine if the actor should start moving
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bStart;

private:
	int32 CurrentPointIndex;
	void MoveToNextPoint();
	//bluprintcallable function to start the movement
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartMovement();
};
