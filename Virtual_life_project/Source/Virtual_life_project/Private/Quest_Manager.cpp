// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest_Manager.h"
#include "Virtual_life_GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"


void UQuest_Manager::ADD_QUEST(const uint8& quest_id)
{
    // NewObject를 사용해 UQuest 객체 생성
    UQuest* NewQuest = NewObject<UQuest>(this, UQuest::StaticClass());
    NewQuest->init_quest(quest_id);
    if (NewQuest)
    {
        Quests.Add(quest_id, NewQuest);
    }
}

bool UQuest_Manager::Quest_complete(const uint8& quest_id)
{
    if (!Quests.Contains(quest_id)) return false;
    return Quests[quest_id]->quest_success();
}

void UQuest_Manager::Delete_Quest(const uint8& quest_id)
{
    Quests.Remove(quest_id);
}

void UQuest_Manager::finish_Quest(const uint8& quest_id)
{
    if (Quests.Contains(quest_id)) {
        Quests[quest_id]->set_complete(true);
    }
}

void UQuest_Manager::Done_Process(const uint8& quest_id)
{
    UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
    if (!game_inst)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance is null or invalid!"));
        return;
    }

    if (!Quests.Contains(quest_id) || Quests[quest_id] == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Quest with ID %d is invalid!"), quest_id);
        return;
    }

    game_inst->SendUpdateGoldPacket(Quests[quest_id]->Quest_info.RewardGold);

    // 플레이어 캐릭터 월드 위치 얻기
    FVector player_location = FVector::ZeroVector;
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            player_location = PlayerPawn->GetActorLocation();
        }
    }

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundWave>(nullptr, TEXT("/Game/sound/effect_sound/cashier.cashier")));

    // 퀘스트 완료 텍스트
    player_location.Z += 40;
    game_inst->ShowFloatingText(FString(TEXT("퀘스트 완료!")), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), player_location);

    // 골드 보상 텍스트
    player_location.Z -= 40;
    game_inst->ShowFloatingText(FString::Printf(TEXT("+%dG"), Quests[quest_id]->Quest_info.RewardGold), FLinearColor(0.97f, 0.95f, 0.3f, 1.0f), player_location);

    game_inst->SendRemoveQuestPacket(0, quest_id);
    
}

int32 UQuest_Manager::get_quests_count()
{
    return Quests.Num();
}
