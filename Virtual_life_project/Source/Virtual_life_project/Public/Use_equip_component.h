#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Use_equip_component.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIRTUAL_LIFE_PROJECT_API UUse_equip_component : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UUse_equip_component();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    UInputComponent* InputComponent; 
    typedef void (UUse_equip_component::* ActionFunc)();
    ActionFunc ActionFunctions[5];

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void SetupInputComponent(class UInputComponent* PlayerInputComponent);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeBindingFunc(uint8 index, uint8 tool_ID);

    void USE_None();
    void USE_fishing_rod();
    void USE_tomato_seed();
    void USE_potato_seed();
    void USE_pickaxe();

    
};
