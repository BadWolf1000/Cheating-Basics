// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
struct gameOffSets
{
    uintptr_t lPlayer = 0xDEF97C;
    uintptr_t fJump = 0x52C0F50;
    uintptr_t flags = 0x104;
    uintptr_t velocity = 0x114;

}offsets;

struct values
{
    uintptr_t localPlayer;
    uintptr_t gameModule;
    BYTE flag;
}val;

struct vector
{
    float x, y, z;
};

bool isPlayerMoving()
{
    vector playerVel = *(vector*)(val.localPlayer + offsets.velocity);
    int vel = playerVel.x + playerVel.y + playerVel.z;
    if (vel != 0)
        return true;
    else 
        return false;
}

void main()
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Injected\n";



    val.gameModule = (uintptr_t)GetModuleHandle("client.dll");
    val.localPlayer = *(uintptr_t*)(val.gameModule + offsets.lPlayer);

    if (val.localPlayer == NULL)
    {
        while (val.localPlayer == NULL)
        {
            val.localPlayer = *(uintptr_t*)(val.gameModule + offsets.lPlayer);
        }
    }
   

    while (true)
    {
        val.flag = *(BYTE*)(val.localPlayer + offsets.flags);

        if (isPlayerMoving())
        {
            if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
            {
                *(uintptr_t*)(val.gameModule + offsets.fJump) = 6;
            }
        }
    }

}


BOOL APIENTRY DllMain(HMODULE hModule, uintptr_t ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);

    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return true;
}
