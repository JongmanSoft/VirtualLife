#include "stdafx.h"
#include "Player.h"

// iocp�� ���õ� ���� ������ //
HANDLE g_iocp_handle;
SOCKET g_server;
SOCKET g_client;

// �� �� //


// �Լ� ���漱�� //
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
        if (ret == FALSE) { // ����!
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
            // todo: id �������ֱ�, LoginID �޾ƿ���
            int client_id = setid();
            CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_client), iocp_hd, client_id, 0);
            cout << "ACCEPT: Ŭ���̾�Ʈ ID " << client_id << " �����" << endl;

            players[client_id] = Player(g_client, client_id);
            players[client_id].recv(); // ù ��° ��Ŷ �ޱ� ����

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
    }
}

int main()
{
    initialize_server();

    // doing acceptEX
    g_client = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    EXT_OVER ac_over;
    ac_over.ov = TASK_TYPE::ACCEPT;
    AcceptEx(g_server, g_client, ac_over.wb_buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, &ac_over.over);

    vector <thread> worker_threads;
    for (int i = 0; i < int(thread::hardware_concurrency()); ++i)
        worker_threads.emplace_back(workerThread, g_iocp_handle);
    for (auto& th : worker_threads)
        th.join();
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

    // ������ �񵿱� ���� ����
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

