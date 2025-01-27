#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <random>
#include <thread>
#include <unordered_set>
#include <concurrent_priority_queue.h>
#include <sqlext.h> 
#include <chrono>
#include <array>
#include <mutex>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>
#include <unordered_map>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")
using namespace std;

#include "protocol.h"
