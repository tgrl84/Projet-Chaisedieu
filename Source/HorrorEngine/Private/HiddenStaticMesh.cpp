


#include "HiddenStaticMesh.h"

void AHiddenStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	mesh = GetStaticMeshComponent();
    bIsCaptured = false; // Initialiser le bool�en
}

void AHiddenStaticMesh::SetIsCaptured(bool bCaptured)
{
    bIsCaptured = bCaptured;
}
