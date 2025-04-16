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


		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Capture")
		bool IsRevealed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		TArray<UMaterialInterface*> TranslucentMats;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		TArray<UMaterialInterface*> NormalMats;

		UFUNCTION(BlueprintCallable, Category = "Capture")
		void SetIsCaptured(bool bCaptured);
		bool getIsCaptured() { return bIsCaptured; }
		void SetMeshVisible() { mesh->SetVisibleInSceneCaptureOnly(false); }
		void SetMeshInvisible() { mesh->SetHiddenInSceneCapture(false); }
		void SetIsRevealed(bool bRevealed);
};
