// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherWidget.h"
#include "Components/Image.h"

void UWeatherWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    // 초기화 - 해 이미지 설정
    SetWeather(Sun_ICN);
}

void UWeatherWidget::NotifySunrise_Implementation()
{
    SetWeather(Sun_ICN);
}

void UWeatherWidget::NotifySunset_Implementation()
{
    SetWeather(Moon_ICN);
}

void UWeatherWidget::SetWeather(UTexture2D* Icon)
{
    if (Weather_IMG && Icon)
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(Icon);
        Weather_IMG->SetBrush(Brush);
    }
}