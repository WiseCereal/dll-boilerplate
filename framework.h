#pragma once

#ifdef _WIN64
	#define ADDRESS_TYPE ULONG64
#else
	#define ADDRESS_TYPE DWORD32
#endif

#include <windows.h>
#include <utility>
#include <map>
#include <stdio.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <intrin.h>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <thread>
#include <bitset>
#include <functional>
#include <psapi.h>