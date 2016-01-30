//
//  main.c
//  External
//
//  Created by XuYang on 16/1/30.
//  Copyright © 2016年 nadp.me. All rights reserved.
//

#include <stdio.h>
#include "Utils.h"
#include <unistd.h>
#include <stdbool.h>


struct Color {
    float red;
    float blue;
    float green;
    float alpha;
};


void applyGlowEffect(task_t task, uint32_t glowStartAddress, int glowObjectIndex, struct Color * color){
    bool stat = 1;
	    vm_write(task, glowStartAddress + 0x38 * glowObjectIndex + 0x24, (vm_offset_t) &stat, sizeof(bool));
    vm_write(task, glowStartAddress + 0x38 * glowObjectIndex + 0x4, (vm_offset_t) &(color->red), 4);
    vm_write(task, glowStartAddress + 0x38 * glowObjectIndex + 0x8, (vm_offset_t) &(color->green), 4);
    vm_write(task, glowStartAddress + 0x38 * glowObjectIndex + 0xc, (vm_offset_t) &(color->blue), 4);
    vm_write(task, glowStartAddress + 0x38 * glowObjectIndex + 0x10, (vm_offset_t) &(color->alpha), 4);
}

void readPlayerPointAndHealth(task_t task, task_t taskSelf, mach_vm_address_t imgbase, uint32_t startAddress, int iTeamNum) {
    int m_iGlowIndex = 0xA2D0;
    uint32_t memoryAddress;
    int glowIndex;
    int playerBase = 0xED1844;
    printf("----------updated----------\n");
    for (int i = 0; i < 0x60; i++) {
        int playerTeamNum;
        if (readIntMam(task, taskSelf, imgbase + playerBase + 0x10 * i, &memoryAddress) == -1) {
            continue;
        }
        if (memoryAddress == 0x0){
            continue;
        }
        if (readIntMam(task, taskSelf, memoryAddress + 0xA2D0, &glowIndex)) {
            continue;
        }
        int health;
        if (readIntMam(task, taskSelf, memoryAddress + 0xf0, &health)) {
            continue;
        }
        if (readIntMam(task, taskSelf, memoryAddress + 0xe4, &playerTeamNum)) {
            continue;
        }
        printf("I teamnum is %i playerteamnum is %i\n", playerTeamNum, iTeamNum);
        if (playerTeamNum == iTeamNum || playerTeamNum == 0) {
            continue;
        }
        if (playerTeamNum == 0) {
            continue;
        }
        struct Color color;
        color.red = (100 - health) / 100.0;
        color.blue = 0.0f;
        color.green = (health) / 100.0;
        color.alpha = 0.8f;
        applyGlowEffect(task, startAddress, glowIndex, &color);
    }
}

void glowInfo(task_t task, task_t current, mach_vm_address_t imgbase, uint32_t * address, int * count){
    int glowInfoOffset = 0xF2C140;
    int glowObjectLoopCount = 0xC;
    readUint32Mam(task, current, imgbase + glowInfoOffset, address);
    readIntMam(task, current, imgbase + glowInfoOffset + glowObjectLoopCount, count);
}


int main(int argc, const char * argv[]) {
    task_t csgo, current;
    int pid = get_process("csgo_osx");
    printf("The pid is %i\n", pid);
    uint32_t * imgBase[2];
    const char * a[2] = {"/client.dylib", "/engine.dylib"};
    task_for_pid(current_task(), getpid(), &current);
    csgo = get_client_module_info(current_task(), current_task(), pid, imgBase, a, 2);
    task_for_pid(current_task(), getpid(), &current);
    uint32_t clientBase = * imgBase;
    uint32_t EngineBase = * (imgBase + 1);
    printf("Client start at 0x%x and engine start at 0x%x", clientBase, EngineBase);
    printf("csgo task is %i pid is %i current task is %i\n", csgo, pid, current);
    // collect info
    int glowObjectLoopCount;
    uint32_t glowObjectLoopStartAddress;
    glowInfo(csgo, current, clientBase, &glowObjectLoopStartAddress, &glowObjectLoopCount);
    printf("glow loop address is 0x%x", glowObjectLoopStartAddress);
    // Apply Glow
    while (1) {
        readPlayerPointAndHealth(csgo, current, clientBase, glowObjectLoopStartAddress, 2);
        //break;
        usleep(20000);
    }
    return 0;
}

