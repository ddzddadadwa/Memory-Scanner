#include "memory.h"

bool Memory::IsStringInMemory(char* string, DWORD process_id, int option) {
	MEMORY_BASIC_INFORMATION memory;
	byte *buffer = (byte*)malloc(BUFFER_SIZE);

	SIZE_T address = 0;
	SIZE_T offset = 0;
	SIZE_T size = 0;
	SIZE_T pos = 0;

	HANDLE pHandle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, process_id);

	INT64 length;
	
	switch (option) {
	case 0:
		// Regular unicode string.
		length = strlen(string);
		break;
	case 1:
		// Since it's a WCHAR we must multiply the length by two to account for the extra chars.
		length = strlen(string) * 2;
		break;
	}

	// Query through all the memory pages.
	while (VirtualQueryEx(pHandle, (LPVOID)address, &memory, sizeof(MEMORY_BASIC_INFORMATION))) {
		// Check for access rights.
		if (memory.State == MEM_COMMIT && memory.Protect != PAGE_NOACCESS && !(memory.Protect & PAGE_GUARD)) {
			offset = 0;

			// Loop through the memory page.
			while (offset < memory.RegionSize) {
				size = min(BUFFER_SIZE, memory.RegionSize - offset);

				// Read the address.
				if (ReadProcessMemory(pHandle, (LPVOID)(address + offset), buffer, size, NULL)) {
					for (SIZE_T i = 0; i < size; i++) {
						// Loop through each char in the string (at i) and see if it equals the same char in the buffer if so, continue looping through the chars.
						if (string[pos] == buffer[i])
							pos++;
						else
							pos = 0;

						// If the looped chars are equal to the buffer chars, we've located out string.
						if (pos == length) {
							// Free the allocated buffer.
							free(buffer);
							return true;
						}
					}
				}
				offset += size;
			}
		}
		address += memory.RegionSize;
	}
	// Free the allocated buffer.
	free(buffer);
	return false;
}