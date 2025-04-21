#include "Room.h"

void Room::AddObject(const Object& obj)
{
    if (objs.size() >= MAX_BUILD_ITEM) return;

    objs.push_back(std::make_shared<Object>(obj));

    std::cout << "Room에 오브젝트 추가됨: ID " << obj.item_id << " 위치 (" << obj.x << ", " << obj.y << ", " << obj.z << ")\n";
}

void Room::packet_setup(SC_ROOM_SETUP_PACKET& pkt)
{
    pkt.size = sizeof(SC_ROOM_SETUP_PACKET);
    pkt.type = SC_ROOM_SETUP;

    int count = min(objs.size(), (size_t)MAX_BUILD_ITEM);
    pkt.count = count;

    for (int i = 0; i < count; ++i)
    {
        pkt.objs[i] = *objs[i]; 
    }
}

void Room::RemoveObjectByPosition(float x, float y, float z)
{
    objs.erase(std::remove_if(objs.begin(), objs.end(),
        [&](std::shared_ptr<Object> obj)
        {
            return fabs(obj->x - x) < 0.1f && fabs(obj->y - y) < 0.1f && fabs(obj->z - z) < 0.1f;
        }),
        objs.end());

    std::cout << "건물 삭제됨\n";
}

void Room::UpdateObjectTransform(float old_x, float old_y, float old_z, float new_x, float new_y, float new_z, float new_yaw)
{
    for (auto& obj : objs)
    {
        if (fabs(obj->x - old_x) < 0.1f &&
            fabs(obj->y - old_y) < 0.1f &&
            fabs(obj->z - old_z) < 0.1f)
        {
            obj->x = new_x;
            obj->y = new_y;
            obj->z = new_z;
            obj->yaw = new_yaw;
            std::cout << "건물 수정됨\n";
            break;
        }
    }
}

void Room::SaveToDB(const std::string& userID)
{
    std::vector<Object> flat_objs;
    for (const auto& obj_ptr : objs)
    {
        if (obj_ptr) {
            flat_objs.push_back(*obj_ptr);  // 포인터 해제해서 복사
        }
    }

    DBManager::SaveRoomObjects(userID, flat_objs);
}

void Room::LoadFromDB(const std::string& userID)
{
    std::vector<Object> loaded_objs;
    if (DBManager::LoadRoomObjects(userID, loaded_objs))
    {
        objs.clear();
        for (const Object& obj : loaded_objs)
        {
            objs.push_back(std::make_shared<Object>(obj));
        }
    }
}