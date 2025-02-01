#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CameroidHitComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORRORENGINE_API UCameroidHitComponent : public USceneComponent
{
    GENERATED_BODY()

    public:
        // Sets default values for this component's properties
        UCameroidHitComponent();

    protected:
        // Called when the game starts
        virtual void BeginPlay() override;

    public:
        // Function to perform a raycast 1000 units in front of the actor callable in blueprint
	    UFUNCTION(BlueprintCallable, Category = "Cameroid")
	    void PerformRaycast(FVector ForwardVector) const;
};
