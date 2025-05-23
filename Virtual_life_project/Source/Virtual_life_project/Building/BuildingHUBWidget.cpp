// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingHUBWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "BuildingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingSelectButtonWidget.h"
#include "Virtual_life_GameInstance.h"
#include "BuildItemRegistry.h"


void UBuildingHUBWidget::NativeConstruct()
{
    Super::NativeConstruct();
    BindTabButtons();
}

void UBuildingHUBWidget::BindTabButtons()
{
    if (FurnitureTab_BTN)
        FurnitureTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnFurnitureTabClicked);

    if (BedroomTab_BTN)
        BedroomTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnBedroomTabClicked);

    if (KitchenTab_BTN)
        KitchenTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnKitchenTabClicked);

    if (StorageTab_BTN)
        StorageTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnStorageTabClicked);

    if (InteriorDecorTab_BTN)
        InteriorDecorTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnInteriorDecorTabClicked);

    if (DecorTab_BTN)
        DecorTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnDecorTabClicked);

    if (StructureTab_BTN)
        StructureTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnStructureTabClicked);

    if (Confirm_BTN)
        Confirm_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnConfirmClicked);

    if(MinjiTheme_BTN)
        MinjiTheme_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnMJBTNClicked);

    if (SeyoungTheme_BTN)
        SeyoungTheme_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnSYBTNClicked);

    if (HaenimTheme_BTN)
        HaenimTheme_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnHNBTNClicked);

    if (SaveTheme_BTN)
        SaveTheme_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnSaveThemeClicked);
}

void UBuildingHUBWidget::OnMJBTNClicked()
{
    // 민지 테마 불러오기
    LoadThemeFromJson(TEXT("minjiTheme.json"));
}

void UBuildingHUBWidget::OnSYBTNClicked()
{
    // 세영 테마 불러오기
    LoadThemeFromJson(TEXT("seyoungTheme.json"));
}

void UBuildingHUBWidget::OnHNBTNClicked()
{
    // 해님 테마 불러오기
    LoadThemeFromJson(TEXT("haenymTheme.json"));
}

void UBuildingHUBWidget::OnSaveThemeClicked()
{
    //if (const auto GI = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(this)))
    //{
    //    const FString CharacterName = GI->GetName();
    //    const FString FileName = CharacterName + TEXT("Theme.json"); //////////////////////////
    //    ExportCurrentThemeToJson(FileName);
    //}
    UE_LOG(LogTemp, Log, TEXT("JSON Button Clicked"));

    const FString FileName = TEXT("Theme.json");
    ExportCurrentThemeToJson(FileName);
}

void UBuildingHUBWidget::OnCategorySelected(EBuildCategories Category)
{
    if (!BuildingDataTable || !WrapBox_Buildings) return;

    WrapBox_Buildings->ClearChildren();

    for (const FName& RowName : BuildingDataTable->GetRowNames())
    {
        const FBuildInfo* Info = BuildingDataTable->FindRow<FBuildInfo>(RowName, "");
        if (Info && Info->Categories == Category)
        {
            UBuildingSelectButtonWidget* Button = CreateWidget<UBuildingSelectButtonWidget>(this, ButtonWidgetClass);
            if (Button)
            {
                Button->SetRowData(BuildingDataTable, RowName);
                WrapBox_Buildings->AddChild(Button);
            }
        }
    }
}

void UBuildingHUBWidget::ExportCurrentThemeToJson(const FString& FileName)
{
    UWorld* World = GetWorld();
    if (!World) {
        UE_LOG(LogTemp, Log, TEXT("JSON - No World"));

        return;
    }

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, APlaceBuildActor::StaticClass(), FoundActors);

    TArray<TSharedPtr<FJsonValue>> JsonArray;

    for (AActor* Actor : FoundActors)
    {
        APlaceBuildActor* Build = Cast<APlaceBuildActor>(Actor);
        if (!Build) continue;

        TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);

        FName RowName = Build->GetRowID();
        JsonObj->SetStringField("item_id", RowName.ToString());

        FVector Loc = Build->GetActorLocation();
        JsonObj->SetNumberField("x", Loc.X);
        JsonObj->SetNumberField("y", Loc.Y);
        JsonObj->SetNumberField("z", Loc.Z);

        JsonObj->SetNumberField("yaw", Build->GetActorRotation().Yaw);

        JsonObj->SetNumberField("scale", Build->GetActorScale3D().X);

        JsonArray.Add(MakeShareable(new FJsonValueObject(JsonObj)));
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonArray, Writer);

    FString SavePath = FPaths::ProjectSavedDir() + "Themes/" + FileName;
    FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*FPaths::GetPath(SavePath)); // 폴더 미리 생성

    if (FFileHelper::SaveStringToFile(OutputString, *SavePath))
    {
        UE_LOG(LogTemp, Log, TEXT("JSON Saved Completed: %s"), *SavePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("JSON Saved Failed!"));
    }
}

void UBuildingHUBWidget::LoadThemeFromJson(const FString& FileName)
{
    // 기존 꾸민거 지우기
    UWorld* World = GetWorld();
    if (!World)
    {
        // UE_LOG(LogTemp, Error, TEXT("No World in LoadThemeFromJson"));
        return;
    }

    TArray<AActor*> ExistingActors;
    UGameplayStatics::GetAllActorsOfClass(World, APlaceBuildActor::StaticClass(), ExistingActors);

    if (auto GI = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        for (AActor* Actor : ExistingActors)
        {
            FVector Location = Actor->GetActorLocation();

            GI->SendRemoveBuildPacket(Location);

            Actor->Destroy();
        }
    }

    const FString Directory = FPaths::Combine(FPaths::LaunchDir(), TEXT("Themes/"));
    const FString FullPath = FPaths::Combine(Directory, FileName);

    FString JsonRaw;
    if (!FFileHelper::LoadFileToString(JsonRaw, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot Load Theme File : %s"), *FullPath);
        return;
    }

    TArray<TSharedPtr<FJsonValue>> JsonArray;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonRaw);

    if (!FJsonSerializer::Deserialize(Reader, JsonArray))
    {
        UE_LOG(LogTemp, Error, TEXT("JSON Failed to parse array: %s"), *FullPath);
        return;
    }

    TArray<FObjectData> LoadedObjects;

    for (const TSharedPtr<FJsonValue>& Value : JsonArray)
    {
        const TSharedPtr<FJsonObject> Obj = Value->AsObject();
        if (!Obj.IsValid()) continue;

        FObjectData Data;

        FString RowNameStr = Obj->GetStringField(TEXT("item_id"));
        FName RowName(*RowNameStr);
        uint16 ID = FBuildItemRegistry::FNameToItemID(RowName);
        Data.ItemID = ID;

        Data.Location.X = Obj->GetNumberField(TEXT("x"));
        Data.Location.Y = Obj->GetNumberField(TEXT("y"));
        Data.Location.Z = Obj->GetNumberField(TEXT("z"));
        Data.Yaw = Obj->GetNumberField(TEXT("yaw"));
        Data.Scale = Obj->GetNumberField(TEXT("scale"));

        LoadedObjects.Add(Data);
    }

    
    if (const auto GI = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->SendPlaceBuildPacket(LoadedObjects);
        GI->SpawnRoomObjectsFromData(LoadedObjects);
    }
    

    UE_LOG(LogTemp, Log, TEXT("%d Object JSON Loaded."), LoadedObjects.Num());
}

void UBuildingHUBWidget::OnFurnitureTabClicked()
{
    OnCategorySelected(EBuildCategories::Furniture);
}

void UBuildingHUBWidget::OnBedroomTabClicked()
{
    OnCategorySelected(EBuildCategories::Bedroom);
}

void UBuildingHUBWidget::OnKitchenTabClicked()
{
    OnCategorySelected(EBuildCategories::Kitchen);
}

void UBuildingHUBWidget::OnStorageTabClicked()
{
    OnCategorySelected(EBuildCategories::Storage);
}

void UBuildingHUBWidget::OnInteriorDecorTabClicked()
{
    OnCategorySelected(EBuildCategories::InteriorDecor);
}

void UBuildingHUBWidget::OnDecorTabClicked()
{
    OnCategorySelected(EBuildCategories::Decor);
}

void UBuildingHUBWidget::OnStructureTabClicked()
{
    OnCategorySelected(EBuildCategories::Structure);
}

void UBuildingHUBWidget::OnConfirmClicked()
{
    ABuildingPlayerController* PC = Cast<ABuildingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC) return;

    PC->ConfirmBuildPlacement();

    UE_LOG(LogTemp, Warning, TEXT("확정 버튼 눌림! 건물 정보 전송."));

    auto GI = Cast<UVirtual_life_GameInstance>(GetGameInstance());
    if (GI)
    {
        GI->EnterMyRoom();
        UE_LOG(LogTemp, Warning, TEXT("Enter My Room Send Completed"));
    }
}
