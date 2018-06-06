#include <windows.h>
#pragma once

// Page size for each memory region, change if you want but it's not recommended 
// as page sizes vary so a high amount to allocate for the buffer  would be a safe choice.
#define BUFFER_SIZE (64 * 1024 * 1024)

class Memory {
public:
	bool IsStringInMemory(char* string, DWORD process_id, int option);
};