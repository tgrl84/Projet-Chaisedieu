// RevealLight.h

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "RevealLight.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORRORENGINE_API URevealLight : public USpotLightComponent
{
    GENERATED_BODY()

public:
    // Constructor
    URevealLight();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void AdjustOpacity(UMaterialInstanceDynamic* DynamicMaterial, float DeltaTime, bool bIncrease, FHitResult* HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reveal")
	float TransitionTime;
};
