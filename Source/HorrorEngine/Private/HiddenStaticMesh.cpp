


#include "HiddenStaticMesh.h"

void AHiddenStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	mesh = GetStaticMeshComponent();
    bIsCaptured = false; // Initialiser le bool�en
	IsRevealed = false;
}

void AHiddenStaticMesh::SetIsCaptured(bool bCaptured)
{
    bIsCaptured = bCaptured;
}

void AHiddenStaticMesh::SetIsRevealed(bool bRevealed) 
{
	IsRevealed = bRevealed;
}