#include "DBManager.h"
#include "RoomManager.h"

sql::mysql::MySQL_Driver* DBManager::g_driver = nullptr;
thread_local std::unique_ptr<sql::Connection> DBManager::t_conn = nullptr;
bool DBManager::DB_ON = true;  // 또는 true, 기본값 지정

std::string WStringToUTF8(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string result(size_needed - 1, 0); // null 제외
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result.data(), size_needed, NULL, NULL);
    return result;
}

std::wstring UTF8ToWString(const std::string& str)
{
    if (str.empty()) return L"";

    // 1차 호출: 필요한 크기 계산
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    // 안전성 검사 추가!
    if (size_needed <= 0) {
        std::cerr << "[UTF8ToWString] MultiByteToWideChar failed, error: " << GetLastError() << std::endl;
        return L"[Invalid UTF-8]";
    }

    // 비정상적으로 큰 크기 방지
    if (size_needed > 10000) {  // 적절한 최대값 설정
        std::cerr << "[UTF8ToWString] Size too large: " << size_needed << std::endl;
        return L"[Too Large]";
    }

    try {
        std::wstring result(size_needed, 0);  // 여기서 터질 수 있음!

        // 2차 호출: 실제 변환
        int converted = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size_needed);
        if (converted <= 0) {
            std::cerr << "[UTF8ToWString] Conversion failed, error: " << GetLastError() << std::endl;
            return L"[Conversion Failed]";
        }

        result.resize(size_needed - 1);  // null terminator 제거
        return result;

    }
    catch (const std::exception& e) {
        std::cerr << "[UTF8ToWString] Exception: " << e.what() << std::endl;
        return L"[Exception]";
    }
}

void DBManager::Init() 
{
    try {
        g_driver = sql::mysql::get_driver_instance();
        if (!g_driver)
            std::cerr << "[DB Init Error] get_driver_instance() returned nullptr" << std::endl;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DB Init Exception] " << e.what() << std::endl;
    }
}

sql::Connection* DBManager::GetConnection() 
{
    try {
        if (!g_driver) {
            std::cerr << "[DB Error] g_driver is null! Init()가 호출되지 않았습니다." << std::endl;
            return nullptr;
        }

        if (!t_conn) {
            t_conn = std::unique_ptr<sql::Connection>(
                g_driver->connect("tcp://127.0.0.1:3306", "root", "12345678"));
            t_conn->setSchema("VL_DB");
        }

        return t_conn.get();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - GetConnection] " << e.what() << std::endl;
        std::cerr << "SQLState: " << e.getSQLStateCStr() << ", ErrorCode: " << e.getErrorCode() << std::endl;
        return nullptr;
    }
    catch (std::exception& e) {
        std::cerr << "[DB Error - GetConnection] std::exception: " << e.what() << std::endl;
        return nullptr;
    }
    catch (...) {
        std::cerr << "[DB Error - GetConnection] 알 수 없는 예외 발생!" << std::endl;
        return nullptr;
    }
}

bool DBManager::checkLogin(const std::string& id, const std::string& pw, bool& is_new)
{
    if (DB_ON == false) return true;
    try {
        sql::Connection* conn = GetConnection();

        // 1. ID 존재 여부 + PW 조회
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT PW FROM LOGIN_INFO WHERE ID=?"));
        stmt->setString(1, id);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (res->next()) {
            // ID 존재 → 저장된 PW와 비교
            std::string db_pw = res->getString("PW");
            is_new = false;
            return db_pw == pw;
        }
        else {
            // ID 존재하지 않음 → INSERT
            std::unique_ptr<sql::PreparedStatement> insertStmt(
                conn->prepareStatement("INSERT INTO LOGIN_INFO (ID, PW, IS_PLAYING) VALUES (?, ?, FALSE)"));
            insertStmt->setString(1, id);
            insertStmt->setString(2, pw);
            insertStmt->executeUpdate();
            is_new = true;
            return true;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error] " << e.what() << std::endl;
        return false;
    }
}

bool DBManager::LoadCustomizing(const std::string& userID, Customizing& outData)
{
    if (DB_ON == false) return true;
    try {
        sql::Connection* conn = GetConnection();

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT * FROM PLAYER_CUSTOM WHERE ID=?"));
        stmt->setString(1, userID);

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (!res->next()) return false; // 데이터 없으면 false

        outData.skin = static_cast<float>(res->getDouble("SKIN"));
        outData.shirt = res->getUInt("SHIRT");
        outData.pants = res->getUInt("PANTS");
        outData.shoes = res->getUInt("SHOES");

        outData.R_eye_color_hue = static_cast<float>(res->getDouble("R_EYE_COLOR_HUE"));
        outData.R_eye_color_sat = static_cast<float>(res->getDouble("R_EYE_COLOR_SAT"));
        outData.L_eye_color_hue = static_cast<float>(res->getDouble("L_EYE_COLOR_HUE"));
        outData.L_eye_color_sat = static_cast<float>(res->getDouble("L_EYE_COLOR_SAT"));
        outData.eye_scale = static_cast<float>(res->getDouble("EYE_SCALE"));
        outData.pupil_scale = static_cast<float>(res->getDouble("PUPIL_SCALE"));

        outData.hair = res->getUInt("HAIR");
        outData.hair_color_R = static_cast<float>(res->getDouble("HAIR_COLOR_R"));
        outData.hair_color_G = static_cast<float>(res->getDouble("HAIR_COLOR_G"));
        outData.hair_color_B = static_cast<float>(res->getDouble("HAIR_COLOR_B"));

        outData.eye_width = static_cast<float>(res->getDouble("EYE_WIDTH"));
        outData.eye_thick = static_cast<float>(res->getDouble("EYE_THICK"));
        outData.eye_slope = static_cast<float>(res->getDouble("EYE_SLOPE"));

        outData.nose_width = static_cast<float>(res->getDouble("NOSE_WIDTH"));
        outData.nose_height = static_cast<float>(res->getDouble("NOSE_HEIGHT"));

        outData.mouse_width = static_cast<float>(res->getDouble("MOUTH_WIDTH"));
        outData.mouse_thick = static_cast<float>(res->getDouble("MOUTH_THICK"));
        outData.mouse_slope = static_cast<float>(res->getDouble("MOUTH_SLOPE"));

        outData.chin = static_cast<float>(res->getDouble("CHIN"));
        outData.jaw = static_cast<float>(res->getDouble("JAW"));
        outData.heavy = static_cast<float>(res->getDouble("HEAVY"));
        outData.face_width = static_cast<float>(res->getDouble("FACE_WIDTH"));
        outData.eyebrows = res->getUInt("EYEBROWS");
        outData.glasses = res->getUInt("GLASSES");

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadCustomizing] " << e.what() << std::endl;
        return false;
    }
}

void DBManager::SaveDefCustomizing(const std::string& id)
{
    Customizing c;
    c.skin = 0.5;
    c.shirt = 0;
    c.pants = 0;
    c.shoes = 0;
    c.R_eye_color_hue = 1;
    c.R_eye_color_sat = 1;
    c.L_eye_color_hue = 1;
    c.L_eye_color_sat = 1;
    c.eye_scale = 0.35f;
    c.pupil_scale = 0.9f;
    c.hair = 0;
    c.hair_color_R = 1;
    c.hair_color_G = 0.2f;
    c.hair_color_B = 1;
    c.eye_width = 0;
    c.eye_thick = 0;
    c.eye_slope = 0;
    c.nose_width = 0;
    c.nose_height = 0;
    c.mouse_width = 0;
    c.mouse_thick = 0;
    c.mouse_slope = 0;
    c.chin = 0;
    c.jaw = 0;
    c.heavy = 0;
    c.face_width = 0;
    c.eyebrows = 0;
    c.glasses = 0;
    SaveCustomizing(id, c);
}

void DBManager::SaveDefPInfo(const std::string& userID, const PlayerInfo& data)
{
    SavePInfo(userID, data, "");
}

void DBManager::SavePInfo(const std::string& userID, const PlayerInfo& data,const std::string& name)
{
    if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                INSERT INTO PLAYER_INFO (ID, NAME, POS_X, POS_Y, POS_Z, YAW, LAST_LOGIN, MONEY)
                VALUES (?, ?, ?, ?, ?, ?, NOW(), ?)
                ON DUPLICATE KEY UPDATE
                    NAME = VALUES(NAME),
                    POS_X = VALUES(POS_X),
                    POS_Y = VALUES(POS_Y),
                    POS_Z = VALUES(POS_Z),
                    YAW = VALUES(YAW),
                    LAST_LOGIN = NOW(),
                    MONEY = VALUES(MONEY)
            )"));

        stmt->setString(1, userID);

        // 이미 UTF-8 string이라면 그대로 사용 가능
        stmt->setString(2, name);

        stmt->setDouble(3, data.x);
        stmt->setDouble(4, data.y);
        stmt->setDouble(5, data.z);
        stmt->setDouble(6, data.yaw);
        stmt->setInt(7, data.gold);

        stmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SavePInfo] " << e.what() << std::endl;
    }
}

void DBManager::SavePInfo(const std::string& userID, const PlayerInfo& data)
{
    if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                UPDATE PLAYER_INFO
                SET POS_X = ?, POS_Y = ?, POS_Z = ?, YAW = ?, LAST_LOGIN = NOW()
                WHERE ID = ?
            )"));

        stmt->setDouble(1, data.x);
        stmt->setDouble(2, data.y);
        stmt->setDouble(3, data.z);
        stmt->setDouble(4, data.yaw);
        stmt->setString(5, userID);

        stmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SavePInfo] " << e.what() << std::endl;
    }
}

bool DBManager::LoadPInfo(const std::string& userID, PlayerInfo& outInfo, std::wstring& outName)
{
    if (DB_ON == false) return true;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) {
            std::cerr << "[DB Error - LoadPInfo] Connection is null!" << std::endl;
            return false;
        }

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                SELECT name, POS_X, POS_Y, POS_Z, YAW
                FROM player_info
                WHERE ID = ?
            )"));

        stmt->setString(1, userID);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (!res->next()) {
            std::cerr << "[DB] LoadPInfo: No entry found for ID = " << userID << std::endl;
            return false;
        }

        std::string name_utf8 = res->getString("name");
        try {
            std::wstring str = UTF8ToWString(name_utf8);
            outName = str;
           // outName = 
        }
        catch (...) {
            outName = L"Unknown";  // 또는 기본값
        }

        outInfo.x = static_cast<float>(res->getDouble("POS_X"));
        outInfo.y = static_cast<float>(res->getDouble("POS_Y"));
        outInfo.z = static_cast<float>(res->getDouble("POS_Z"));
        outInfo.yaw = static_cast<float>(res->getDouble("YAW"));

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadPInfo] " << e.what() << std::endl;
        return false;
    }
}

void DBManager::SavePname(const std::string& userID, const std::wstring& name)
{
    if (DB_ON == false) return;
	try {
		sql::Connection* conn = GetConnection();
		std::unique_ptr<sql::PreparedStatement> stmt(
			conn->prepareStatement(R"(
                UPDATE PLAYER_INFO
                SET NAME = ?
                WHERE ID = ?
            )"));
		// wstring을 string으로 변환
        std::string name_utf8 = WStringToUTF8(name);
		stmt->setString(1, name_utf8);
		stmt->setString(2, userID);
		stmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		std::cerr << "[DB Error - SavePname] " << e.what() << std::endl;
	}
}

void DBManager::SaveGold(const std::string& userID, const int gold)
{
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("UPDATE player_info SET MONEY = ? WHERE ID = ?"));
        pstmt->setInt(1, gold);
        pstmt->setString(2, userID);

        pstmt->execute();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveGold] " << e.what() << std::endl;
    }
}

int DBManager::LoadGold(const std::string& userID)
{
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return 0; 

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT MONEY FROM player_info WHERE ID = ?"));
        pstmt->setString(1, userID);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return res->getInt("MONEY");
        }
        else {
            std::cerr << "[DB Warning - LoadGold] 유저 " << userID << "의 GOLD 정보 없음\n";
            return 0;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadGold] " << e.what() << std::endl;
        return 0;
    }
}


void DBManager::SaveCustomizing(const std::string& userID, const Customizing& data)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
        INSERT INTO PLAYER_CUSTOM (
            ID, SKIN, SHIRT, PANTS, SHOES,
            R_EYE_COLOR_HUE, R_EYE_COLOR_SAT,
            L_EYE_COLOR_HUE, L_EYE_COLOR_SAT,
            EYE_SCALE, PUPIL_SCALE,
            HAIR, HAIR_COLOR_R, HAIR_COLOR_G, HAIR_COLOR_B,
            EYE_WIDTH, EYE_THICK, EYE_SLOPE,
            NOSE_WIDTH, NOSE_HEIGHT,
            MOUTH_WIDTH, MOUTH_THICK, MOUTH_SLOPE,
            CHIN, JAW, HEAVY, FACE_WIDTH, EYEBROWS, GLASSES
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?,?)
        ON DUPLICATE KEY UPDATE
            SKIN=VALUES(SKIN), SHIRT=VALUES(SHIRT), PANTS=VALUES(PANTS), SHOES=VALUES(SHOES),
            R_EYE_COLOR_HUE=VALUES(R_EYE_COLOR_HUE), R_EYE_COLOR_SAT=VALUES(R_EYE_COLOR_SAT),
            L_EYE_COLOR_HUE=VALUES(L_EYE_COLOR_HUE), L_EYE_COLOR_SAT=VALUES(L_EYE_COLOR_SAT),
            EYE_SCALE=VALUES(EYE_SCALE), PUPIL_SCALE=VALUES(PUPIL_SCALE),
            HAIR=VALUES(HAIR), HAIR_COLOR_R=VALUES(HAIR_COLOR_R),
            HAIR_COLOR_G=VALUES(HAIR_COLOR_G), HAIR_COLOR_B=VALUES(HAIR_COLOR_B),
            EYE_WIDTH=VALUES(EYE_WIDTH), EYE_THICK=VALUES(EYE_THICK), EYE_SLOPE=VALUES(EYE_SLOPE),
            NOSE_WIDTH=VALUES(NOSE_WIDTH), NOSE_HEIGHT=VALUES(NOSE_HEIGHT),
            MOUTH_WIDTH=VALUES(MOUTH_WIDTH), MOUTH_THICK=VALUES(MOUTH_THICK), MOUTH_SLOPE=VALUES(MOUTH_SLOPE),
            CHIN=VALUES(CHIN), JAW=VALUES(JAW), HEAVY=VALUES(HEAVY), FACE_WIDTH=VALUES(FACE_WIDTH),
            EYEBROWS=VALUES(EYEBROWS),GLASSES=VALUES(GLASSES)
    )"));

        int i = 1;
        stmt->setString(i++, userID);
        stmt->setDouble(i++, data.skin);
        stmt->setUInt(i++, data.shirt);
        stmt->setUInt(i++, data.pants);
        stmt->setUInt(i++, data.shoes);

        stmt->setDouble(i++, data.R_eye_color_hue);
        stmt->setDouble(i++, data.R_eye_color_sat);
        stmt->setDouble(i++, data.L_eye_color_hue);
        stmt->setDouble(i++, data.L_eye_color_sat);
        stmt->setDouble(i++, data.eye_scale);
        stmt->setDouble(i++, data.pupil_scale);

        stmt->setUInt(i++, data.hair);
        stmt->setDouble(i++, data.hair_color_R);
        stmt->setDouble(i++, data.hair_color_G);
        stmt->setDouble(i++, data.hair_color_B);

        stmt->setDouble(i++, data.eye_width);
        stmt->setDouble(i++, data.eye_thick);
        stmt->setDouble(i++, data.eye_slope);

        stmt->setDouble(i++, data.nose_width);
        stmt->setDouble(i++, data.nose_height);

        stmt->setDouble(i++, data.mouse_width);
        stmt->setDouble(i++, data.mouse_thick);
        stmt->setDouble(i++, data.mouse_slope);

        stmt->setDouble(i++, data.chin);
        stmt->setDouble(i++, data.jaw);
        stmt->setDouble(i++, data.heavy);
        stmt->setDouble(i++, data.face_width);

        stmt->setUInt(i++, data.eyebrows);
        stmt->setUInt(i++, data.glasses);

        stmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveCustom] " << e.what() << std::endl;
    }
}

void DBManager::SaveItem(const std::string& userID, int itemCode, int itemCount)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();

        if (itemCount <= 0)
        {
            // 0개 이하일 경우 삭제
            std::unique_ptr<sql::PreparedStatement> delStmt(
                conn->prepareStatement("DELETE FROM player_inventory WHERE ID=? AND ITEM_CODE=?"));
            delStmt->setString(1, userID);
            delStmt->setInt(2, itemCode);
            delStmt->executeUpdate();
        }
        else
        {
            // 삽입 또는 갱신
            std::unique_ptr<sql::PreparedStatement> stmt(
                conn->prepareStatement(R"(
                    INSERT INTO player_inventory (ID, ITEM_CODE, ITEM_COUNT)
                    VALUES (?, ?, ?)
                    ON DUPLICATE KEY UPDATE ITEM_COUNT = VALUES(ITEM_COUNT)
                )"));
            stmt->setString(1, userID);
            stmt->setInt(2, itemCode);
            stmt->setInt(3, itemCount);
            stmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveItem] " << e.what() << std::endl;
    }
}

bool DBManager::LoadItem(const std::string& userID, std::unordered_map<unsigned short, unsigned short>& outData)
{
	if (DB_ON == false) return true;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) {
            std::cerr << "[DB Error - LoadItem] Connection is null!" << std::endl;
            return false;
        }

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT ITEM_CODE, ITEM_COUNT FROM player_inventory WHERE ID = ?"));
        stmt->setString(1, userID);

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        outData.clear();  // 기존 데이터 초기화

        while (res->next()) {
            unsigned short code = static_cast<unsigned short>(res->getInt("ITEM_CODE"));
            unsigned short count = static_cast<unsigned short>(res->getInt("ITEM_COUNT"));
            outData[code] = count;
        }

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadItem] " << e.what() << std::endl;
        return false;
    }
}

void DBManager::SaveRoomObject(const std::string& userID, const Object& object)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::ostringstream query;
        query << "INSERT INTO player_room (ID, ITEM_ID, POS_X, POS_Y, POS_Z, SCALE, YAW) VALUES ";

            query << "('" << userID << "', "
                << object.item_id << ", "
                << object.x << ", "
                << object.y << ", "
                << object.z << ", "
                << object.scale << ", "
                << object.yaw << ")";

        query << " ON DUPLICATE KEY UPDATE "
            << "SCALE=VALUES(SCALE), YAW=VALUES(YAW)";

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute(query.str());
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveRoomObject(Bulk)] " << e.what() << std::endl;
    }
}

void DBManager::UpdateRoomObject(
    const std::string& userID,
    float oldX, float oldY, float oldZ,
    float newX, float newY, float newZ,
    float newYaw
)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                UPDATE player_room
                SET POS_X = ?, POS_Y = ?, POS_Z = ?, YAW = ?
                WHERE ID = ?
                AND ABS(POS_X - ?) < 0.001
                AND ABS(POS_Y - ?) < 0.001
                AND ABS(POS_Z - ?) < 0.001
            )"));

        // SET 절 (새 위치와 새 YAW)
        stmt->setDouble(1, newX);
        stmt->setDouble(2, newY);
        stmt->setDouble(3, newZ);
        stmt->setDouble(4, newYaw);

        // WHERE 절 (기존 값 비교)
        stmt->setString(5, userID);
        stmt->setDouble(6, oldX);
        stmt->setDouble(7, oldY);
        stmt->setDouble(8, oldZ);

        stmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - UpdateRoomObjectPositionAndYaw] " << e.what() << std::endl;
    }
}


bool DBManager::LoadRoomObjects(const std::string& userID, std::vector<Object>& outObjects)
{
	if (DB_ON == false) return true;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return false;

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                SELECT ITEM_ID, POS_X, POS_Y, POS_Z, SCALE, YAW
                FROM player_room
                WHERE ID = ?
            )"));
        stmt->setString(1, userID);

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        outObjects.clear();  // 기존 데이터 초기화

        while (res->next())
        {
            Object obj;
            obj.item_id = res->getUInt("ITEM_ID");
            obj.x = static_cast<float>(res->getDouble("POS_X"));
            obj.y = static_cast<float>(res->getDouble("POS_Y"));
            obj.z = static_cast<float>(res->getDouble("POS_Z"));
            obj.scale = static_cast<float>(res->getDouble("SCALE"));
            obj.yaw = static_cast<float>(res->getDouble("YAW"));

            if (obj.item_id == DUMMY_OBJECT_ID) continue;
            outObjects.emplace_back(obj);
        }

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadRoomObjects] " << e.what() << std::endl;
        return false;
    }
}

void DBManager::DeleteRoomObject(const std::string& userID, float posX, float posY, float posZ)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                DELETE FROM player_room
                WHERE ID = ?
                AND ABS(POS_X - ?) < 0.001
                AND ABS(POS_Y - ?) < 0.001
                AND ABS(POS_Z - ?) < 0.001
            )"));

        stmt->setString(1, userID);
        stmt->setDouble(2, posX);
        stmt->setDouble(3, posY);
        stmt->setDouble(4, posZ);

        stmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - DeleteRoomObject] " << e.what() << std::endl;
    }
}

void DBManager::SaveRoomObjects(const std::string& userID, const std::vector<Object>& objects)
{
	if (DB_ON == false) return;
    if (objects.empty()) return;

    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::ostringstream query;
        query << "INSERT INTO player_room (ID, ITEM_ID, POS_X, POS_Y, POS_Z, SCALE, YAW) VALUES ";

        for (size_t i = 0; i < objects.size(); ++i) {
            const Object& obj = objects[i];
            query << "('" << userID << "', "
                << obj.item_id << ", "
                << obj.x << ", "
                << obj.y << ", "
                << obj.z << ", "
                << obj.scale << ", "
                << obj.yaw << ")";
            if (i != objects.size() - 1) {
                query << ", ";
            }
        }

        query << " ON DUPLICATE KEY UPDATE "
            << "SCALE=VALUES(SCALE), YAW=VALUES(YAW), "
            << "POS_X=VALUES(POS_X), POS_Y=VALUES(POS_Y), POS_Z=VALUES(POS_Z)";

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute(query.str());
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveRoomObjects(Bulk)] " << e.what() << std::endl;
    }
}

void DBManager::DeleteRoomObjects(const std::string& userID)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::ostringstream query;
        query << "DELETE FROM player_room WHERE ID = '" << userID << "'";

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute(query.str());
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - DeleteRoomObjectsByUser] " << e.what() << std::endl;
    }
}

bool DBManager::LoadQuest(const std::string& userID, std::vector<Quest>& outData)
{
	if (DB_ON == false) return true;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return false;

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT QUEST_ID FROM player_quest WHERE ID = ?"));
        pstmt->setString(1, userID);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            int quest_id = res->getInt("QUEST_ID");
            outData.emplace_back(0, quest_id); 
        }

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadQuest] " << e.what() << std::endl;
        return false;
    }
}

void DBManager::SaveQuest(const std::string& userID, Quest& data)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO player_quest (ID, QUEST_ID) VALUES (?, ?) "
                "ON DUPLICATE KEY UPDATE QUEST_ID = VALUES(QUEST_ID)"));
        pstmt->setString(1, userID);
        pstmt->setInt(2, data.GetNUM());  // 혹은 적절한 getter 함수 사용
        
        pstmt->execute();
        return;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveQuest] " << e.what() << std::endl;
        return;
    }
}

void DBManager::DeleteQuest(const std::string& userID, Quest& data)
{
	if (DB_ON == false) return;
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return;

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("DELETE FROM player_quest WHERE ID = ? AND QUEST_ID = ?"));
        pstmt->setString(1, userID);
        pstmt->setInt(2, data.GetNUM());  // 적절한 getter 함수 사용

        pstmt->execute();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - DeleteQuest] " << e.what() << std::endl;
    }
}

void DBManager::SaveKidInfo(const Kid& kid)
{
    if (!DB_ON) return;
    try {
        auto conn = GetConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(R"(
            INSERT INTO kid_info (
                ID, PREG_ID, SPOUSE_ID, PERSONALITY, HELLO_MSG,
                SKIN, SHIRT, PANTS, SHOES,
                R_EYE_COLOR_HUE, R_EYE_COLOR_SAT, L_EYE_COLOR_HUE, L_EYE_COLOR_SAT,
                EYE_SCALE, PUPIL_SCALE, HAIR, HAIR_COLOR_R, HAIR_COLOR_G, HAIR_COLOR_B,
                EYE_WIDTH, EYE_THICK, EYE_SLOPE,
                NOSE_WIDTH, NOSE_HEIGHT,
                MOUTH_WIDTH, MOUTH_THICK, MOUTH_SLOPE,
                CHIN, JAW, HEAVY, FACE_WIDTH, EYEBROWS, GLASSES,
                x, y, z, yaw, name, is_Kid
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            ON DUPLICATE KEY UPDATE
                PREG_ID = VALUES(PREG_ID),
                SPOUSE_ID = VALUES(SPOUSE_ID),
                PERSONALITY = VALUES(PERSONALITY),
                HELLO_MSG = VALUES(HELLO_MSG),
                SKIN = VALUES(SKIN),
                SHIRT = VALUES(SHIRT),
                PANTS = VALUES(PANTS),
                SHOES = VALUES(SHOES),
                R_EYE_COLOR_HUE = VALUES(R_EYE_COLOR_HUE),
                R_EYE_COLOR_SAT = VALUES(R_EYE_COLOR_SAT),
                L_EYE_COLOR_HUE = VALUES(L_EYE_COLOR_HUE),
                L_EYE_COLOR_SAT = VALUES(L_EYE_COLOR_SAT),
                EYE_SCALE = VALUES(EYE_SCALE),
                PUPIL_SCALE = VALUES(PUPIL_SCALE),
                HAIR = VALUES(HAIR),
                HAIR_COLOR_R = VALUES(HAIR_COLOR_R),
                HAIR_COLOR_G = VALUES(HAIR_COLOR_G),
                HAIR_COLOR_B = VALUES(HAIR_COLOR_B),
                EYE_WIDTH = VALUES(EYE_WIDTH),
                EYE_THICK = VALUES(EYE_THICK),
                EYE_SLOPE = VALUES(EYE_SLOPE),
                NOSE_WIDTH = VALUES(NOSE_WIDTH),
                NOSE_HEIGHT = VALUES(NOSE_HEIGHT),
                MOUTH_WIDTH = VALUES(MOUTH_WIDTH),
                MOUTH_THICK = VALUES(MOUTH_THICK),
                MOUTH_SLOPE = VALUES(MOUTH_SLOPE),
                CHIN = VALUES(CHIN),
                JAW = VALUES(JAW),
                HEAVY = VALUES(HEAVY),
                FACE_WIDTH = VALUES(FACE_WIDTH),
                EYEBROWS = VALUES(EYEBROWS),
                GLASSES = VALUES(GLASSES),
                x = VALUES(x), 
                y = VALUES(y), 
                z = VALUES(z),
                yaw = VALUES(yaw),
                name = VALUES(name),
                is_Kid = VALUES(is_Kid)
        )"));

        pstmt->setUInt(1, kid.id);
        pstmt->setUInt(2, kid.preg_id);
        pstmt->setUInt(3, kid.spouse_id);
        pstmt->setInt(4, kid.personality);
        pstmt->setString(5, WStringToUTF8(kid.hello_msg));

        const Customizing& c = kid.customizing;
        pstmt->setDouble(6, c.skin);
        pstmt->setInt(7, c.shirt);
        pstmt->setInt(8, c.pants);
        pstmt->setInt(9, c.shoes);
        pstmt->setDouble(10, c.R_eye_color_hue);
        pstmt->setDouble(11, c.R_eye_color_sat);
        pstmt->setDouble(12, c.L_eye_color_hue);
        pstmt->setDouble(13, c.L_eye_color_sat);
        pstmt->setDouble(14, c.eye_scale);
        pstmt->setDouble(15, c.pupil_scale);
        pstmt->setDouble(16, c.hair);
        pstmt->setDouble(17, c.hair_color_R);
        pstmt->setDouble(18, c.hair_color_G);
        pstmt->setDouble(19, c.hair_color_B);
        pstmt->setDouble(20, c.eye_width);
        pstmt->setDouble(21, c.eye_thick);
        pstmt->setDouble(22, c.eye_slope);
        pstmt->setDouble(23, c.nose_width);
        pstmt->setDouble(24, c.nose_height);
        pstmt->setDouble(25, c.mouse_width); 
        pstmt->setDouble(26, c.mouse_thick); 
        pstmt->setDouble(27, c.mouse_slope); 
        pstmt->setDouble(28, c.chin);
        pstmt->setDouble(29, c.jaw);
        pstmt->setDouble(30, c.heavy);
        pstmt->setDouble(31, c.face_width);
        pstmt->setInt(32, c.eyebrows);
        pstmt->setInt(33, c.glasses);
        pstmt->setDouble(34, kid.x);
        pstmt->setDouble(35, kid.y);
        pstmt->setDouble(36, kid.z);
        pstmt->setDouble(37, kid.yaw);
        pstmt->setString(38, WStringToUTF8(kid.name));
        pstmt->setInt(39, kid.is_kid);  // 39번째 파라미터로 수정

        pstmt->executeUpdate();
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DB] SaveKidInfo 예외 발생: " << e.what() << std::endl;
    }
}


bool DBManager::LoadKidInfo(unsigned int id, Kid& outKid)
{
    if (!DB_ON) return false;

    try {
        auto conn = GetConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT * FROM kid_info WHERE ID = ?"
        ));
        pstmt->setUInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            outKid.id = res->getUInt("ID");
            outKid.preg_id = res->getUInt("PREG_ID");
            outKid.spouse_id = res->getUInt("SPOUSE_ID");
            outKid.personality = static_cast<char>(res->getInt("PERSONALITY"));

            outKid.hello_msg = UTF8ToWString(res->getString("HELLO_MSG"));

            Customizing& c = outKid.customizing;
            c.skin = static_cast<float>(res->getDouble("SKIN"));
            c.shirt = static_cast<unsigned short>(res->getInt("SHIRT"));
            c.pants = static_cast<unsigned short>(res->getInt("PANTS"));
            c.shoes = static_cast<unsigned short>(res->getInt("SHOES"));
            c.R_eye_color_sat = static_cast<float>(res->getDouble("R_EYE_COLOR_SAT"));
            c.R_eye_color_hue = static_cast<float>(res->getDouble("R_EYE_COLOR_HUE"));
            c.L_eye_color_hue = static_cast<float>(res->getDouble("L_EYE_COLOR_HUE"));
            c.L_eye_color_sat = static_cast<float>(res->getDouble("L_EYE_COLOR_SAT"));
            c.eye_scale = static_cast<float>(res->getDouble("EYE_SCALE"));
            c.pupil_scale = static_cast<float>(res->getDouble("PUPIL_SCALE"));
            c.hair = static_cast<unsigned short>(res->getInt("HAIR"));
            c.hair_color_R = static_cast<float>(res->getDouble("HAIR_COLOR_R"));
            c.hair_color_G = static_cast<float>(res->getDouble("HAIR_COLOR_G"));
            c.hair_color_B = static_cast<float>(res->getDouble("HAIR_COLOR_B"));
            c.eye_width = static_cast<float>(res->getDouble("EYE_WIDTH"));
            c.eye_thick = static_cast<float>(res->getDouble("EYE_THICK"));
            c.eye_slope = static_cast<float>(res->getDouble("EYE_SLOPE"));
            c.nose_width = static_cast<float>(res->getDouble("NOSE_WIDTH"));
            c.nose_height = static_cast<float>(res->getDouble("NOSE_HEIGHT"));
            c.mouse_width = static_cast<float>(res->getDouble("MOUTH_WIDTH"));
            c.mouse_thick = static_cast<float>(res->getDouble("MOUTH_THICK"));
            c.mouse_slope = static_cast<float>(res->getDouble("MOUTH_SLOPE"));
            c.chin = static_cast<float>(res->getDouble("CHIN"));
            c.jaw = static_cast<float>(res->getDouble("JAW"));
            c.heavy = static_cast<float>(res->getDouble("HEAVY"));
            c.face_width = static_cast<float>(res->getDouble("FACE_WIDTH"));
            c.eyebrows = res->getInt("EYEBROWS");
            c.glasses = res->getInt("GLASSES");

            outKid.x = static_cast<float>(res->getDouble("x"));
            outKid.y = static_cast<float>(res->getDouble("y"));
            outKid.z = static_cast<float>(res->getDouble("z"));
            outKid.yaw = static_cast<float>(res->getDouble("yaw"));
            outKid.name = UTF8ToWString(res->getString("name"));
            outKid.is_kid = res->getInt("is_Kid") != 0;

            return true;
        }
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DB] LoadKidInfo 예외 발생: " << e.what() << std::endl;
    }

    return false;
}


bool DBManager::LoadAllRoomsFromDB()
{
    if (!DB_ON) return false;

    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return false;

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(R"(
            SELECT ID, ITEM_ID, POS_X, POS_Y, POS_Z, SCALE, YAW
            FROM player_room
        )"));

        while (res->next()) {
            std::string userID = res->getString("ID");

            // Room이 없다면 새로 생성
            if (RoomManager::Get().find(userID) == RoomManager::Get().end()) {
                RoomManager::Get().insert({ userID, new Room() });
                RoomManager::Get()[userID]->ownerID = userID;
            }

            // Object 생성
            Object obj;
            obj.item_id = res->getInt("ITEM_ID");
            obj.x = static_cast<float>(res->getDouble("POS_X"));
            obj.y = static_cast<float>(res->getDouble("POS_Y"));
            obj.z = static_cast<float>(res->getDouble("POS_Z"));
            obj.scale = static_cast<float>(res->getDouble("SCALE"));
            obj.yaw = static_cast<float>(res->getDouble("YAW"));

            // Room에 추가
            RoomManager::Get()[userID]->AddObject(obj);
        }

        std::cout << "[INFO] Loaded " << RoomManager::Get().size() << " rooms from DB." << std::endl;
        return true;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[ERROR] Failed to load rooms: " << e.what() << std::endl;
    }
    return false;
}

bool DBManager::LoadAllKidsFromDB()
{
    if (!DB_ON) return false;

    try {
        sql::Connection* conn = GetConnection();
        if (!conn) return false;

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(R"(
            SELECT *
            FROM kid_info
        )"));

        while (res->next()) {
            Kid kid;

            kid.id = res->getUInt("ID");
            kid.preg_id = res->getUInt("PREG_ID");
            kid.spouse_id = res->getUInt("SPOUSE_ID");
            kid.personality = static_cast<char>(res->getInt("PERSONALITY"));

            kid.hello_msg = UTF8ToWString(res->getString("HELLO_MSG"));

            Customizing& c = kid.customizing;
            c.skin = static_cast<float>(res->getDouble("SKIN"));
            c.shirt = res->getInt("SHIRT");
            c.pants = res->getInt("PANTS");
            c.shoes = res->getInt("SHOES");

            c.R_eye_color_hue = static_cast<float>(res->getDouble("R_EYE_COLOR_HUE"));
            c.R_eye_color_sat = static_cast<float>(res->getDouble("R_EYE_COLOR_SAT"));
            c.L_eye_color_hue = static_cast<float>(res->getDouble("L_EYE_COLOR_HUE"));
            c.L_eye_color_sat = static_cast<float>(res->getDouble("L_EYE_COLOR_SAT"));

            c.eye_scale = static_cast<float>(res->getDouble("EYE_SCALE"));
            c.pupil_scale = static_cast<float>(res->getDouble("PUPIL_SCALE"));
            c.hair = static_cast<short>(res->getInt("HAIR"));
            c.hair_color_R = static_cast<float>(res->getDouble("HAIR_COLOR_R"));
            c.hair_color_G = static_cast<float>(res->getDouble("HAIR_COLOR_G"));
            c.hair_color_B = static_cast<float>(res->getDouble("HAIR_COLOR_B"));

            c.eye_width = static_cast<float>(res->getDouble("EYE_WIDTH"));
            c.eye_thick = static_cast<float>(res->getDouble("EYE_THICK"));
            c.eye_slope = static_cast<float>(res->getDouble("EYE_SLOPE"));
            c.nose_width = static_cast<float>(res->getDouble("NOSE_WIDTH"));
            c.nose_height = static_cast<float>(res->getDouble("NOSE_HEIGHT"));

            c.mouse_width = static_cast<float>(res->getDouble("MOUTH_WIDTH"));
            c.mouse_thick = static_cast<float>(res->getDouble("MOUTH_THICK"));
            c.mouse_slope = static_cast<float>(res->getDouble("MOUTH_SLOPE"));

            c.chin = static_cast<float>(res->getDouble("CHIN"));
            c.jaw = static_cast<float>(res->getDouble("JAW"));
            c.heavy = static_cast<float>(res->getDouble("HEAVY"));
            c.face_width = static_cast<float>(res->getDouble("FACE_WIDTH"));

            c.eyebrows = res->getInt("EYEBROWS");
            c.glasses = res->getInt("GLASSES");

            kid.x = static_cast<float>(res->getDouble("x"));
            kid.y = static_cast<float>(res->getDouble("y"));
            kid.z = static_cast<float>(res->getDouble("z"));
            kid.yaw = static_cast<float>(res->getDouble("yaw"));

            std::string nameStr = res->getString("name");
            kid.name = UTF8ToWString(nameStr);

            kid.is_kid = res->getInt("is_Kid");

            npcs[kid.id] = kid;
            npc_count++;
        }

        std::cout << "[INFO] Loaded " << npc_count << " kids from DB." << std::endl;
        return true;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "[DB] LoadAllKidInfo 예외 발생: " << e.what() << std::endl;
    }

    return false;
}
