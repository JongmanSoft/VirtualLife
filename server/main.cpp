#include "stdafx.h"
#include "Player.h"
#include "DBManager.h"

// iocp에 관련된 전역 변수들 //
HANDLE g_iocp_handle;
SOCKET g_server;
SOCKET g_client;

// 그 외 //
concurrency::concurrent_priority_queue<EVENT> g_evt_queue;

// 함수 전방선언 //
void initialize_server();
void push_evt_queue(int from, int to, TASK_TYPE ev, int time);
bool room_setup();
bool door_setup();

void workerThread(HANDLE iocp_hd)
{
    while (true)
    {
        DWORD num_bytes;
        ULONG_PTR key;
        WSAOVERLAPPED* over;
        BOOL ret;
        ret = GetQueuedCompletionStatus(iocp_hd, &num_bytes, &key, &over, INFINITE);
        if (ret == FALSE) { // 실패!
            if (over == nullptr) {
                printf("GetQueuedCompletionStatus failed with error: %d\n", GetLastError());
            }
            continue;
        }

        int player_id = static_cast<int>(key);
        EXT_OVER* ext_over = reinterpret_cast<EXT_OVER*>(over);
        if (player_id < 0)
            player_id = (player_id) * (-1) - 1;

        if (ext_over->ov == TASK_TYPE::ACCEPT) {
            int client_id = setid();
            CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_client), iocp_hd, client_id, 0);
            std::cout << "[ACCEPT] 클라이언트 ID " << client_id << " 연결됨" << std::endl;
            
            {
				std::lock_guard<std::mutex> lock(players_mutex);
                players[client_id].init_player(g_client, client_id);
            }
            players[client_id].recv(); // 첫 번째 패킷 받기 시작

            g_client = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

            EXT_OVER ac_over;
            ac_over.ov = TASK_TYPE::ACCEPT;
            AcceptEx(g_server, g_client, ac_over.wb_buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, &ac_over.over);
        }
        else if (ext_over->ov == TASK_TYPE::RECV)
        {
            Player& player = players[player_id];
            player.update_packet(ext_over, num_bytes);
            player.process_buffer();
            player.recv();
        }
        else if (ext_over->ov == TASK_TYPE::SEND)
        {
            delete ext_over;
        }
        else if (ext_over->ov == TASK_TYPE::DB_POS_UPDATE)
        {
            std::cout << "[EVENT] DB_UPDATE - 플레이어 위치 저장" << std::endl;

            {
                std::lock_guard<std::mutex> lock(players_mutex);
                for (Player& player : players)
                {
                    if (player.get_state() != PLAYING)
                        continue;
                    player.save_db_pinfo();
                }
            }

            push_evt_queue(-1, -1, TASK_TYPE::DB_POS_UPDATE, DB_POS_UPDATE_TIME); // 10분 뒤 저장
        }
        else if (ext_over->ov == TASK_TYPE::DB_INVENTORY_UPDATE)
        {
            std::cout << "[EVENT] DB_INVENTORY_UPDATE - 플레이어 인벤토리 저장" << std::endl;

            {
                std::lock_guard<std::mutex> lock(players_mutex);
                for (Player& player : players)
                {
                    if (player.get_state() != PLAYING)
                        continue;
                    player.save_db_pInventory();
                }
            }

            push_evt_queue(-1, -1, TASK_TYPE::DB_INVENTORY_UPDATE, DB_INVENTORY_UPDATE_TIME);
        }
        else if (ext_over->ov == TASK_TYPE::TIME_UPDATE) // todo
        {
            // 모든 아이들에게 현재 시간 전송
            for (Player& player : players) 
            {
                if (player.get_state() != PLAYING)
                    continue;
            }
        }
    }
}

void check_evt(HANDLE iocp_hd)
{
    while (true)
    {
        EVENT ev;
        bool event_processed = false;

        if (g_evt_queue.try_pop(ev))
        {
            if (ev.GETTIME() <= std::chrono::system_clock::now())
            {
                EXT_OVER* ex_over = new EXT_OVER();
                ex_over->ov = ev.evt_type;
                ex_over->from = ev.from_id;
                ex_over->to = ev.to_id;

                PostQueuedCompletionStatus(iocp_hd, 0, ev.to_id, &ex_over->over);
            }
            else
            {
                g_evt_queue.push(ev);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
}

int main()
{
    // 서버 초기화
    initialize_server();

	if (DBManager::DB_ON == true)
	{
        DBManager::Init();

        push_evt_queue(-1, -1, TASK_TYPE::DB_POS_UPDATE, DB_POS_UPDATE_TIME);
        push_evt_queue(-1, -1, TASK_TYPE::DB_INVENTORY_UPDATE, DB_INVENTORY_UPDATE_TIME);
        room_setup();
    }

    // doing acceptEX
    g_client = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    EXT_OVER ac_over;
    ac_over.ov = TASK_TYPE::ACCEPT;
    AcceptEx(g_server, g_client, ac_over.wb_buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, &ac_over.over);

    std::vector<std::thread> worker_threads;

    // 0420 이벤트 큐 추가
    std::thread evt_thread{ check_evt, g_iocp_handle };
    
    for (int i = 0; i < int(std::thread::hardware_concurrency()); ++i)
        worker_threads.emplace_back(workerThread, g_iocp_handle);
    for (auto& th : worker_threads)
        th.join();
    closesocket(g_server);
    WSACleanup();
}

void initialize_server()
{
    f_time = 15.0f;
    startTime = std::chrono::high_resolution_clock::now();

    setlocale(LC_ALL, "korean");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        server_error("WSAStartup failed");

    g_server = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (g_server == INVALID_SOCKET)
        server_error("WSASocket failed");

    // 소켓을 비동기 모드로 설정
    u_long mode = 1;
    if (ioctlsocket(g_server, FIONBIO, &mode) != NO_ERROR) {
        server_error("ioctlsocket failed");
    }

    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_NUM);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(g_server, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
        server_error("bind failed");

    if (listen(g_server, SOMAXCONN) == SOCKET_ERROR)
        server_error("listen failed");

    g_iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (g_iocp_handle == NULL)
        server_error("CreateIoCompletionPort failed");

    if (CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_server), g_iocp_handle, 0, 0) == NULL)
        server_error("CreateIoCompletionPort for server socket failed");
}

void push_evt_queue(int from, int to, TASK_TYPE ev, int time) // time: milisecond 단위.
{
    EVENT evt;
    evt.setup(ev, time, from, to);
    g_evt_queue.push(evt);
}

bool room_setup()
{
    bool result = DBManager::LoadAllRoomsFromDB();
    if (false == result)
    {
		std::cerr << "[DB Error] LoadAllRoomsFromDB failed!" << std::endl;
        return false;
    }
    return true;
}

bool door_setup()
{
	for (int i = 0; i < MAX_DOOR; ++i)
	{
		doors[i].id = i;
		doors[i].is_open = false;
	}
	return true;
}