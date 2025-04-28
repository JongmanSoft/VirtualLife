#include "DBManager.h"

sql::mysql::MySQL_Driver* DBManager::g_driver = nullptr;
thread_local std::unique_ptr<sql::Connection> DBManager::t_conn = nullptr;


void DBManager::Init() 
{
    if (!g_driver) {
        g_driver = sql::mysql::get_driver_instance();
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
    try {
        sql::Connection* conn = GetConnection();

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT * FROM PLAYER_CUSTOM WHERE ID=?"));
        stmt->setString(1, userID);

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (!res->next()) return false; // 데이터 없으면 false

        outData.skin = res->getDouble("SKIN");
        outData.shirt = res->getUInt("SHIRT");
        outData.pants = res->getUInt("PANTS");
        outData.shoes = res->getUInt("SHOES");

        outData.R_eye_color_hue = res->getDouble("R_EYE_COLOR_HUE");
        outData.R_eye_color_sat = res->getDouble("R_EYE_COLOR_SAT");
        outData.L_eye_color_hue = res->getDouble("L_EYE_COLOR_HUE");
        outData.L_eye_color_sat = res->getDouble("L_EYE_COLOR_SAT");
        outData.eye_scale = res->getDouble("EYE_SCALE");
        outData.pupil_scale = res->getDouble("PUPIL_SCALE");

        outData.hair = res->getUInt("HAIR");
        outData.hair_color_R = res->getDouble("HAIR_COLOR_R");
        outData.hair_color_G = res->getDouble("HAIR_COLOR_G");
        outData.hair_color_B = res->getDouble("HAIR_COLOR_B");

        outData.eye_width = res->getDouble("EYE_WIDTH");
        outData.eye_thick = res->getDouble("EYE_THICK");
        outData.eye_slope = res->getDouble("EYE_SLOPE");

        outData.nose_width = res->getDouble("NOSE_WIDTH");
        outData.nose_height = res->getDouble("NOSE_HEIGHT");

        outData.mouse_width = res->getDouble("MOUTH_WIDTH");
        outData.mouse_thick = res->getDouble("MOUTH_THICK");
        outData.mouse_slope = res->getDouble("MOUTH_SLOPE");

        outData.chin = res->getDouble("CHIN");
        outData.jaw = res->getDouble("JAW");
        outData.heavy = res->getDouble("HEAVY");
        outData.face_width = res->getDouble("FACE_WIDTH");

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
    c.eye_scale = 0.35;
    c.pupil_scale = 0.9;
    c.hair = 0;
    c.hair_color_R = 1;
    c.hair_color_G = 0.2;
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
    SaveCustomizing(id, c);
}

void DBManager::SaveDefPInfo(const std::string& userID, const PlayerInfo& data)
{
    SavePInfo(userID, data, "");
}

void DBManager::SavePInfo(const std::string& userID, const PlayerInfo& data, const std::string& name)
{
    try {
        sql::Connection* conn = GetConnection();

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                INSERT INTO PLAYER_INFO (ID, NAME, POS_X, POS_Y, POS_Z, YAW, LAST_LOGIN)
                VALUES (?, ?, ?, ?, ?, ?, NOW())
                ON DUPLICATE KEY UPDATE
                    NAME = VALUES(NAME),
                    POS_X = VALUES(POS_X),
                    POS_Y = VALUES(POS_Y),
                    POS_Z = VALUES(POS_Z),
                    YAW = VALUES(YAW),
                    LAST_LOGIN = NOW()
            )"));

        stmt->setString(1, userID);

        // 이미 UTF-8 string이라면 그대로 사용 가능
        stmt->setString(2, name);

        stmt->setDouble(3, data.x);
        stmt->setDouble(4, data.y);
        stmt->setDouble(5, data.z);
        stmt->setDouble(6, data.yaw);

        stmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SavePInfo] " << e.what() << std::endl;
    }
}

void DBManager::SavePInfo(const std::string& userID, const PlayerInfo& data)
{
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
    try {
        sql::Connection* conn = GetConnection();
        if (!conn) {
            std::cerr << "[DB Error - LoadPInfo] Connection is null!" << std::endl;
            return false;
        }

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(R"(
                SELECT NAME, POS_X, POS_Y, POS_Z, YAW
                FROM player_info
                WHERE ID = ?
            )"));

        stmt->setString(1, userID);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (!res->next()) {
            std::cerr << "[DB] LoadPInfo: No entry found for ID = " << userID << std::endl;
            return false;
        }

        // 문자열 변환 (UTF-8 → wstring)
        std::string name_utf8 = res->getString("NAME");
        outName = std::wstring(name_utf8.begin(), name_utf8.end());

        outInfo.x = res->getDouble("POS_X");
        outInfo.y = res->getDouble("POS_Y");
        outInfo.z = res->getDouble("POS_Z");
        outInfo.yaw = res->getDouble("YAW");

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadPInfo] " << e.what() << std::endl;
        return false;
    }
}

void DBManager::SaveCustomizing(const std::string& userID, const Customizing& data)
{
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
            CHIN, JAW, HEAVY, FACE_WIDTH
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
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
            CHIN=VALUES(CHIN), JAW=VALUES(JAW), HEAVY=VALUES(HEAVY), FACE_WIDTH=VALUES(FACE_WIDTH)
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

        stmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveCustom] " << e.what() << std::endl;
    }
}

void DBManager::SaveItem(const std::string& userID, int itemCode, int itemCount)
{
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

void DBManager::SaveRoomObjects(const std::string& userID, const std::vector<Object>& objects)
{
    try {
        sql::Connection* conn = GetConnection();
        if (!conn || objects.empty()) return;

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
            if (i < objects.size() - 1)
                query << ", ";
        }

        query << " ON DUPLICATE KEY UPDATE "
            << "SCALE=VALUES(SCALE), YAW=VALUES(YAW)";

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute(query.str());
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - SaveRoomObjects(Bulk)] " << e.what() << std::endl;
    }
}

bool DBManager::LoadRoomObjects(const std::string& userID, std::vector<Object>& outObjects)
{
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
            obj.x = res->getDouble("POS_X");
            obj.y = res->getDouble("POS_Y");
            obj.z = res->getDouble("POS_Z");
            obj.scale = res->getDouble("SCALE");
            obj.yaw = res->getDouble("YAW");

            outObjects.emplace_back(obj);
        }

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error - LoadRoomObjects] " << e.what() << std::endl;
        return false;
    }
}
