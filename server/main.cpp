#include "stdafx.h"

// iocp에 관련된 전역 변수들 //
HANDLE g_iocp_handle;
SOCKET g_server;
SOCKET g_client;

// 그 외 //

// 함수 전방선언 //
void initialize_server();

void workerThread(HANDLE iocp_hd)
{
    while (true)
    {
        DWORD num_bytes;
        ULONG_PTR key;
        WSAOVERLAPPED* over;
        BOOL ret;
        ret = GetQueuedCompletionStatus(iocp_hd, &num_bytes, &key, &over, INFINITE);
        if (ret == FALSE)
        {
            if (over == nullptr)
            {
                printf("GetQueuedCompletionStatus failed with error: %d\n", GetLastError());

                continue;
            }

            // 요기를 굳이 해야 할까
            int player_id = static_cast<int>(key);
            Player& player = players[player_id];
            int error = WSAGetLastError();

            DB_user_logout(string(player.getName()), hstmt);

            printf("Client [%d] encountered an error: %d\n", player_id, error);
            players[player_id].state = NONE; // todo: state lock?
            continue;
        }

        int player_id = static_cast<int>(key);
        EXT_OVER* ext_over = reinterpret_cast<EXT_OVER*>(over);
        if (player_id < 0)
            player_id = (player_id) * (-1) - 1;

        if (ext_over->ov == TASK_TYPE::ACCEPT)
        {
            cout << "ACCEPT: " << g_client << endl;
            int client_id = setid();
            cout << client_id << endl;
            if (client_id != -1) {
                Player& player = players[client_id];
                // lock을 여기 해야 할까
                player.setup(client_id, g_client);
                CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_client), iocp_hd, client_id, 0);
                player.recv();
                g_client = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
            }
            else
                cout << "Max user exceeded.\n";

            EXT_OVER ac_over; // 괜찮을까?
            ac_over.ov = TASK_TYPE::ACCEPT;
            AcceptEx(g_server, g_client, ac_over.wb_buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, &ac_over.over);
        }
        else if (ext_over->ov == TASK_TYPE::RECV)
        {
            Player& player = players[player_id];
            player.update_packet(ext_over, num_bytes);
            player.process_buffer(hstmt);

            player.recv();
        }
        else if (ext_over->ov == TASK_TYPE::SEND)
        {
            delete ext_over;
        }
        else if (ext_over->ov == TASK_TYPE::DB_UPDATE) // DB에 user 값들 업데이트.
        {
            for (Player& p : players)
            {
                if (p.get_state() != PLAYING) continue;
                const char* user_name = p.getName();
                short user_x = p.getX();
                short user_y = p.getY();
                int user_exp = p.getEXP();

                wchar_t wquery[512];
                swprintf_s(wquery, sizeof(wquery) / sizeof(wquery[0]),
                    L"EXEC UpdateUserDataByName @user_name = '%S', @user_x = %d, @user_y = %d, @user_exp = %d;",
                    user_name, user_x, user_y, user_exp);

                ret = SQLExecDirect(hstmt, wquery, SQL_NTS);
                if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
                    printf("Query executed successfully.\n");
                else
                    printf("Error executing query.\n");
            }
            push_evt_queue(-1, -1, TASK_TYPE::EV_DB_UPDATE, 60000); // 300으로 고쳐야함
        }
        else if (ext_over->ov == TASK_TYPE::RANDOM_MOVE) // 그냥 랜덤 무브
        {
            if (npcs[player_id].hp <= 0) break;
            npcs[player_id].setState(false);

            int t = (player_id + 1) * -1;
            for (auto& a : players)
            {
                if (a.get_state() == PLAYING and a.isNear(t)) {
                    npcs[player_id].randomMove();
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(-0.2, 0.2);
                    npcs[player_id].setState(true);
                    if ((npcs[player_id].target_id == -1 and npcs[player_id].movement == Roaming) or npcs[player_id].type == Peace)
                        push_evt_queue(t, t, TASK_TYPE::EV_RANDOM_MOVE, static_cast<int>((1 + dis(gen)) * 1000)); // 랜덤시간 이동
                    else
                        push_evt_queue(a.id, t, TASK_TYPE::EV_FOLLOW_MOVE, 1000);
                    break;
                }
            }
            delete ext_over;
        }
        else if (ext_over->ov == TASK_TYPE::FOLLOW_MOVE)
        {
            if (npcs[player_id].hp <= 0) break;
            npcs[player_id].setState(false);

            int t = (player_id + 1) * -1;
            for (auto& a : players)
            {
                npcs[player_id].follow_move();
                npcs[player_id].setState(true);
                if (npcs[player_id].target_id == -1)
                    if (npcs[player_id].isOrigin() and npcs[player_id].movement == Roaming)
                        push_evt_queue(t, t, TASK_TYPE::EV_RANDOM_MOVE, 1000);
                    else
                        push_evt_queue(t, t, TASK_TYPE::EV_FOLLOW_MOVE, 1000);
                else
                    push_evt_queue(a.id, t, TASK_TYPE::EV_FOLLOW_MOVE, 1500);
                break;
            }


            delete ext_over;
        }
        else if (ext_over->ov == TASK_TYPE::HEAL)
        {
            if (static_cast<int>(key) < 0)
            {
                if (npcs[player_id].do_healing())
                    push_evt_queue(static_cast<int>(key), static_cast<int>(key), TASK_TYPE::EV_HEAL, 5000);

                for (const auto& n : Nears) {
                    short sx = npcs[player_id].sector_x + n.x;
                    short sy = npcs[player_id].sector_y + n.y;

                    if (sx >= 0 && sx <= W_WIDTH / SECTOR_SIZE && sy >= 0 && sy <= W_HEIGHT / SECTOR_SIZE) {
                        std::lock_guard<std::mutex> sector_ll(g_SectorLock);
                        for (auto& i : g_SectorList[sx][sy]) {
                            if (!npcs[player_id].isNear(i)) continue;
                            if (i < 0) continue;
                            if (players[i].get_state() != PLAYING) continue;
                            players[i].send_stat_change(0, npcs[player_id].hp, npcs[player_id].level, npcs[player_id].max_hp, npcs[player_id].id);  // 상태변환.
                        }
                    }
                }
            }
            else
            {
                if (players[player_id].do_healing())
                    push_evt_queue(static_cast<int>(key), static_cast<int>(key), TASK_TYPE::EV_HEAL, 5000);
                for (const auto& n : Nears) {
                    short sx = players[player_id].sector_x + n.x;
                    short sy = players[player_id].sector_y + n.y;

                    if (sx >= 0 && sx <= W_WIDTH / SECTOR_SIZE && sy >= 0 && sy <= W_HEIGHT / SECTOR_SIZE) {
                        std::lock_guard<std::mutex> sector_ll(g_SectorLock);
                        for (auto& i : g_SectorList[sx][sy]) {
                            if (!players[player_id].isNear(i)) continue;
                            if (i < 0) continue;
                            if (players[i].get_state() != PLAYING) continue;
                            players[i].send_stat_change(0, players[player_id].hp, players[player_id].level, players[player_id].max_hp, players[player_id].id);  // 상태변환.
                        }
                    }
                }
            }

            delete ext_over;
        }
        else if (ext_over->ov == TASK_TYPE::RESPAWN)
        {
            if (static_cast<int>(key) < 0)
                npcs[player_id].respawn();
            else
                players[player_id].respawn();
            delete ext_over;
        }
    }

    DB_disconnect(hdbc, hstmt);
}

int main()
{
    initialize_server();

    // doing acceptEX
    g_client = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    EXT_OVER ac_over;
    ac_over.ov = TASK_TYPE::ACCEPT;
    AcceptEx(g_server, g_client, ac_over.wb_buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, &ac_over.over);

    // add threads
    thread evt_thread{ check_evt, g_iocp_handle }; // PostQueuedCompletionStatus 하는 애
    vector <thread> worker_threads;
    for (int i = 0; i < int(thread::hardware_concurrency()); ++i)
        worker_threads.emplace_back(wk_thread, g_iocp_handle);
    for (auto& th : worker_threads)
        th.join();
    evt_thread.join();
    closesocket(g_server);
    WSACleanup();
}

void initialize_server()
{
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

