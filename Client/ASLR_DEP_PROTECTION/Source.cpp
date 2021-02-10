#include <Windows.h>
#include <imagehlp.h>
#include <iostream>
#include <string>
#pragma comment(lib,"imagehlp.lib")

bool FLAG(const char* path, bool ASLR, bool DEP) {
    LOADED_IMAGE PE;
    if (MapAndLoad(path, 0, &PE, 0, 0)) { // Maps an image and preloads data from the mapped file.
        if (ASLR) {
            // enabling the flag of ASLR by OR on the flag (which mean that if the flag is already True, nothing happens)
            PE.FileHeader->OptionalHeader.DllCharacteristics |= IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
        }
        else {
            // // disabling the flag of ASLR by AND on the flag (which mean that if the flag is already Flase, nothing happens)
            PE.FileHeader->OptionalHeader.DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
        }

        if (DEP) {
            //enable
            PE.FileHeader->OptionalHeader.DllCharacteristics |= IMAGE_DLLCHARACTERISTICS_NX_COMPAT;
        }
        else {
            //disable
            PE.FileHeader->OptionalHeader.DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_NX_COMPAT;
        }
        UnMapAndLoad(&PE);
        return true;
    }
    else
        return false;
}

std::wstring ExePath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

void main() {
    if (FLAG("Client.exe", true, false))
        std::cout << "Flags successfuly checked and set!" << std::endl;
    else
        std::cout << "Invalid path or PE format!" << std::endl;
}

