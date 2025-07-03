// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character_interface.h"
#include "InteractableActor.generated.h"

class UBoxComponent;
class UUserWidget;

UCLASS()
class AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void HandleInteract();

    void StartInteraction();
    void EndInteraction();

protected:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> InteractWidgetClass;

    UPROPERTY()
    UUserWidget* W_Interact;

    UPROPERTY()
    AActor* InteractingActor;

    UPROPERTY()
    bool bIsInteracting;

};
