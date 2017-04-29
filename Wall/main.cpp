//
//  main.cpp
//  Wall
//
//  for csgo wall hack
//  special edition
//
//  Created by sheep on 16/5/14.
//  Copyright © 2016年 nadp. All rights reserved.
//

/**
 *
 * @toggle key : CTRL + ALT (OPT) + V
 * Change modifier at line 226
 * Change keycode at line 227
 *
 */

#include <iostream>
#include <ApplicationServices/ApplicationServices.h>
#include "memory/process.cpp"

pid_t   mainPid     = -1;
task_t  mainTask    = 0;

#include "memory/manager.cpp"
#include "memory/scanner.cpp"

bool ctr = false;
bool sft = false;
bool cmd = false;
bool alt = false;
bool states = true;

#include "utils/keyboard.h"

uint64_t glowInfoOffset;
uint64_t LocalPlayerBase;
uint64_t playerBase;

uint64_t m_iGlowIndex       = 0xAC10;
bool statBool = true;

struct Color {
    float red;
    float green;
    float blue;
    float alpha;
};

void applyEntityGlow(mach_vm_address_t imgbase, mach_vm_address_t startAddress, int iTeamNum)
{
    for (int i = 0; i < 60; i++) {
        uint64_t memoryAddress  = mem->read<uint64_t>(imgbase + playerBase + 0x20 * i);

        if (memoryAddress <= 0x0){
            continue;
        }
        
        int glowIndex           = mem->read<int>(memoryAddress + m_iGlowIndex);
        int health              = mem->read<int>(memoryAddress + 0x134);
        int playerTeamNum       = mem->read<int>(memoryAddress + 0x128);

        if (playerTeamNum == iTeamNum || playerTeamNum == 0) {
            continue;
        }
        
        if (playerTeamNum == 0) {
            continue;
        }

        if (health == 0) {
            health = 100;
        }
        
        Color color = {float((100 - health) / 100.0), float((health) / 100.0), 0.0f, 0.8f};
        
        // We don't need to call an extra function to just write to memory (Keep the code clean)
        uint64_t glowBase = startAddress + (0x40 * glowIndex);
        
        mem->write<bool>(glowBase + 0x28, statBool);
        mem->write<Color>(glowBase + 0x8, color);
    }
}

int main(int argc, const char * argv[])
{
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_BACKGROUND, 0), ^{
        keyBoardListen();
    });
    
    mainPid     = g_cProc->get("csgo_osx64");
    mainTask    = g_cProc->task(mainPid);

    if (mainPid == -1) {
        printf("Cant find the PID of CSGO\n");
        exit(0);
    } else {
        printf("Found CSGO PID: %i\n", mainPid);
    }
    
    off_t moduleLength = 0;
    mach_vm_address_t moduleStartAddress;
    g_cProc->getModule(mainTask, &moduleStartAddress, &moduleLength, "/client.dylib");

    if (mainTask) {
        printf("Found the Client.dylib address: 0x%llx \n", moduleStartAddress);
        printf("Module should end at 0x%llx\n", moduleStartAddress + moduleLength);
    } else {
        printf("Failed to get the Client.dylib address\n");
        exit(0); // we exit here because there is no task for this pid
    }
    
    Scanner * clientScanner = new Scanner(moduleStartAddress, moduleLength);
    
    LocalPlayerBase = clientScanner->getPointer(
        (Byte*)"\x89\xD6\x41\x89\x00\x49\x89\x00\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x00",
        "xxxx?xx?xxx????xxxx?",
        0xB
    ) + 0x4;
    
    playerBase = clientScanner->getPointer(
        (Byte*)"\x48\x8D\x1D\x00\x00\x00\x00\x48\x89\xDF\xE8\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x89\xDE",
        "xxx????xxxx????xxx????xxx????xxx",
        0x3
    ) + 0x2C;
    
    glowInfoOffset = clientScanner->getPointer(
        (Byte*)"\x48\x8D\x3D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x48\x8D\x35\x00\x00\x00\x00",
        "xxx????x????xxxx????xxx????xxxxxxx????",
        0x22
    ) + 0x4;
    
    uint64_t glowObjectLoopStartAddress = mem->read<uint64_t>(moduleStartAddress + glowInfoOffset);
    while (mainPid != -1 && mainTask != 0) // run hack until CS:GO is running.
    {
        if (states)
        {
            // We read the localPlayer Entity directly in the while loop, as we might need for other features
            uint64_t playerAddress  = mem->read<uint64_t>(moduleStartAddress + LocalPlayerBase);
            // Developer Comment:
            // int health              = mem->read<int>(playerAddress + 0x134);
            // We don't read this because it was never been used
            int i_teamNum = mem->read<int>(playerAddress + 0x128);
            
            applyEntityGlow(moduleStartAddress, glowObjectLoopStartAddress, i_teamNum);
        }
        usleep(3000); // decreased this as the most performance issues should be fixed
    }
    
    // When we get here we will exit. CS:GO is not running any more.
    std:system("clear");
    printf("CS:GO closed. Please restart CS:GO and the hack.");
    exit(0);
}
