// Fill out your copyright notice in the Description page of Project Settings.


#include "ProfileCaptureFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include "ImageUtils.h"

UTexture2D* UProfileCaptureFunctionLibrary::ConvertRenderTargetToTexture(UTextureRenderTarget2D* RenderTarget)
{
    if (!RenderTarget) return nullptr;

    FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();

    TArray<FColor> Pixels;
    RTResource->ReadPixels(Pixels);

    int32 Width = RenderTarget->SizeX;
    int32 Height = RenderTarget->SizeY;

    UTexture2D* NewTexture = FImageUtils::CreateTexture2D(
        Width,
        Height,
        Pixels,
        nullptr,
        TEXT("CapturedProfileTexture"),
        RF_NoFlags,
        FCreateTexture2DParameters()
    );

    return NewTexture;
}