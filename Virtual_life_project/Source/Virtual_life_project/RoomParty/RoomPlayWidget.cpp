// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPlayWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Virtual_life_GameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URoomPlayWidget::NativeConstruct()
{
	Super::NativeConstruct();



	if (EnterBuildModeButton)
		EnterBuildModeButton->OnClicked.AddDynamic(this, &URoomPlayWidget::OnEnterBuildModeClicked);

	if (QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &URoomPlayWidget::OnQuitClicked);


}

void URoomPlayWidget::OnEnterBuildModeClicked()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("BuildingMap")));

}

void URoomPlayWidget::OnQuitClicked()
{
	
	UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
	if (my_instance)
	{
		my_instance->SendRoomLeavePacket();
	}
}

void URoomPlayWidget::disable_enter_building()
{
	UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
	if (my_instance) {
		if (strcmp(my_instance->user_id, my_instance->current_room)!=0) {
			EnterBuildModeButton->SetVisibility(ESlateVisibility::Collapsed); // 버튼 숨기기
			EnterBuildModeButton->SetIsEnabled(false); // 버튼 기능 비활성화
			edit_text->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
}
