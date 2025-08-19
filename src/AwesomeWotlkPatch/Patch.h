#pragma once
#include <vector>


struct PatchDetails {
    unsigned virtualAddress;
    const char* hexBytes;
};

PatchDetails s_patches[] = {
    {
        0x004DCCF0, // lua_ScanDllStart
        "B8" "00000000" // mov eax, 1
        "C3"            // ret
    },
    {
        0x004E5CB0, // ScanDllStart
        "B8" "01000000" // mov eax, 1
        "A3" "74B4B600" // mov s_isScanDllFinished, eax
        "68" "E05C4E00" // push AwesomeWotlkLib.dll
        "E8" "1C683800" // call _loadddll
        "83C4" "04" // add esp, 4
        "55" // push ebp
        "8BEC" // mov ebp, esp
        "E8" "A110F2FF" // call 0x00406D70
        "E9" "045BF2FF" // jmp 0x0040B7D8
        "CCCCCCCCCCCCCCCCCCCCCCCC" // int3 (12 times)
        "417765736F6D65576F746C6B4C69622E646C6C00" // AwesomeWotlkLib.dll
    },
    {
        0x0040B7D0, // StartAddress
        "E9" "DBA40D00" // jmp 0x004E5CB0
        "909090" // nop (3 times)
    }
};

PatchDetails s_patches_ascension[] = {
    {
        0x009E425D,
        "417765736F6D65576F746C6B4C69622E646C6C00" // "AwesomeWotlkLib.dll\0"
    },
    {
        0x007744CC,
        "68" "5D429E00"   // push 0x009E425D
        "FF15" "48F29D00" // call dword ptr [KERNEL32.LoadLibraryA]
        // Original instructions
        "61"              // popad
        "E9" "C5A8F1FF"   // jmp FUN_0068EDA2
    }
};
