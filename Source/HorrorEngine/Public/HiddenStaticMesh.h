

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "HiddenStaticMesh.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORRORENGINE_API AHiddenStaticMesh : public AStaticMeshActor
{
	GENERATED_BODY()
	
	protected:
		virtual void BeginPlay() override;
	public:
		UStaticMeshComponent* mesh;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capture")
		bool bIsCaptured;

		UFUNCTION(BlueprintCallable, Category = "Capture")
		void SetIsCaptured(bool bCaptured);
		bool getIsCaptured() { return bIsCaptured; }
		void SetMeshVisible() { mesh->SetVisibleInSceneCaptureOnly(false); }
		void SetMeshInvisible() { mesh->SetHiddenInSceneCapture(false); }
};
