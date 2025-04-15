#include "RevealLight.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/SpotLightComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include <HiddenStaticMesh.h>

URevealLight::URevealLight()
{
    PrimaryComponentTick.bCanEverTick = true;
    TransitionTime = 5.0f; // Dur�e par d�faut pour rendre le mat�riau transparent
}

void URevealLight::BeginPlay()
{
    Super::BeginPlay();
}

void URevealLight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (this->IsVisible())
    {
        FVector Start = GetComponentLocation() + (GetForwardVector() * 50);
        FVector ForwardVector = GetForwardVector();
        FVector End = Start + (ForwardVector * 1000.0f);

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(GetOwner());

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,
            CollisionParams
        );

        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

        if (bHit && HitResult.GetComponent())
        {
            DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10, FColor::Red, false, 1);

            if (HitResult.GetComponent()->ComponentTags.Num() > 0)
            {
                AHiddenStaticMesh* HiddenMeshActor = Cast<AHiddenStaticMesh>(HitResult.GetActor());
                if (HitResult.GetComponent()->ComponentTags.Contains(FName("Revealable")) && HiddenMeshActor->getIsCaptured())
                {
                    HiddenMeshActor->SetMeshVisible();
                    UE_LOG(LogTemp, Log, TEXT("Mesh visibility changed for actor: %s"), *HiddenMeshActor->GetName());

                    UMaterialInterface* Material = HitResult.GetComponent()->GetMaterial(0);
                    if (Material)
                    {
                        UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(HitResult.GetComponent()->GetMaterial(0));
                        if (DynamicMaterial)
                        {
                            UE_LOG(LogTemp, Log, TEXT("Dynamic material found for actor: %s"), *HiddenMeshActor->GetName());

                            if (HitResult.GetComponent()->ComponentTags.Contains(FName("0To1")))
                            {
                                // Augmenter progressivement l'opacit�
                                AdjustOpacity(DynamicMaterial, DeltaTime, true, &HitResult);
                            }
                            else if (HitResult.GetComponent()->ComponentTags.Contains(FName("1To0")))
                            {
                                // R�duire progressivement l'opacit�
                                AdjustOpacity(DynamicMaterial, DeltaTime, false, &HitResult);
                            }
                        }
                        else
                        {
                            UE_LOG(LogTemp, Warning, TEXT("No dynamic material found for actor: %s"), *HiddenMeshActor->GetName());
                        }
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Hit actor is not revealable or not captured: %s"), *HitResult.GetActor()->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Hit Actor has no tags: %s"), *HitResult.GetActor()->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("No hit detected"));
        }
    }
}

void URevealLight::AdjustOpacity(UMaterialInstanceDynamic* DynamicMaterial, float DeltaTime, bool bIncrease, FHitResult* HitResult)
{
    // R�cup�rer l'opacit� actuelle � partir du mat�riau dynamique
    float CurrentOpacity;
    DynamicMaterial->GetScalarParameterValue(FName("Opacity"), CurrentOpacity);

    float OpacityIncrement = DeltaTime / TransitionTime;

    // Augmenter ou diminuer l'opacit�
    if (bIncrease)
    {
        // Augmenter l'opacit� progressivement jusqu'� 1
        CurrentOpacity = FMath::Clamp(CurrentOpacity + OpacityIncrement, 0.0f, 1.0f);
        if (CurrentOpacity == 1.0f)
        {
            // Activer un collider bloquant le joueur
            HitResult->GetComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

            // D�finir le canal de collision et les r�ponses
            HitResult->GetComponent()->SetCollisionObjectType(ECC_WorldDynamic);
            HitResult->GetComponent()->SetCollisionResponseToAllChannels(ECR_Block);

            HitResult->GetComponent()->ComponentTags.Remove(FName("Revealable"));
        }
    }
    else
    {
        // R�duire l'opacit� progressivement jusqu'� 0
        CurrentOpacity = FMath::Clamp(CurrentOpacity - OpacityIncrement, 0.0f, 1.0f);
        if (CurrentOpacity == 0.0f)
        {
            // D�sactiver les collisions pour le composant
            HitResult->GetComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            HitResult->GetComponent()->ComponentTags.Remove(FName("Revealable"));
        }
    }

    // Appliquer la nouvelle opacit� au mat�riau dynamique
    DynamicMaterial->SetScalarParameterValue("Opacity", CurrentOpacity);

    UE_LOG(LogTemp, Log, TEXT("Current Opacity: %f"), CurrentOpacity);
}
