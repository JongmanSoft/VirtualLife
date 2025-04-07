// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyProfileWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UMyProfileWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    /*UFUNCTION(BlueprintCallable)
    void SetProfileName(const FString& Name);*/

    /*UFUNCTION(BlueprintCallable)
    void SetJobName(const FString& Job);*/

    // 레벨업이 있을 수 잇으니
    UFUNCTION(BlueprintCallable)
    void SetHP(int32 Current, int32 Max);

    UFUNCTION(BlueprintCallable)
    void SetMP(int32 Current, int32 Max);

protected:
    virtual void NativeConstruct() override;

    // 텍스트 바인딩
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Name_TXT;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Job_TXT;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HP_TXT;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MP_TXT;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Coin_TXT;

    // 프로그레스 바 바인딩
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HP_Bar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* MP_Bar;
};
