#include "stdafx.h"

atomic_int id = 0;

int setid()
{
    return id++;
}

void server_error(const char* msg)
{
    printf("%s with error: %d\n", msg, WSAGetLastError());
    exit(1);
}
