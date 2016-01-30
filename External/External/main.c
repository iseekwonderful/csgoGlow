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

task_t csgo, current;
uint32_t clientBase,EngineBase;
struct Color {
    float red;
    float blue;
    float green;
    float alpha;
};


void applyGlowEffect(uint32_t glowStartAddress, int glowObjectIndex, struct Color * color){
    bool stat = 1;
	vm_write(csgo, glowStartAddress + 0x38 * glowObjectIndex + 0x24, (vm_offset_t) &stat, sizeof(bool));
    vm_write(csgo, glowStartAddress + 0x38 * glowObjectIndex + 0x4, (vm_offset_t) &color, sizeof(Color));
}

void readPlayerPointAndHealth(mach_vm_address_t imgbase, uint32_t startAddress, int iTeamNum) {
    int m_iGlowIndex = 0xA2D0;
    uint32_t memoryAddress;
    int glowIndex;
    int playerBase = 0xED1844;
    printf("----------updated----------\n");
    for (int i = 0; i < 0x60; i++) {
        if (readIntMam(csgo, current, imgbase + playerBase + 0x10 * i, &memoryAddress) == -1)//Entetiy address
            continue;
        if (memoryAddress == 0x0)//is nullpointer
            continue;
        if (readIntMam(csgo, current, memoryAddress + 0xA2D0, &glowIndex)) //Read GlowIndex
            continue;
        int health;
        if (readIntMam(csgo, current, memoryAddress + 0xf0, &health)) //Read Health
            continue;
		if(health == 0)
			continue;
		int playerTeamNum;
        if (readIntMam(csgo, current, memoryAddress + 0xe4, &playerTeamNum)) //Read Team Number
            continue;
        if (playerTeamNum == 0 || playerTeamNum == iTeamNum || playerTeamNum == 0) 
            continue;
        struct Color color;
        color.red = (100 - health) / 100.0;
        color.blue = 0.0f;
        color.green = (health) / 100.0;
        color.alpha = 0.8f;
        applyGlowEffect(csgo, startAddress, glowIndex, &color);
    }
}

void glowInfo(mach_vm_address_t imgbase, uint32_t * address, int * count){
    int glowInfoOffset = 0xF2C140;
    int glowObjectLoopCount = 0xC;
    readUint32Mam(csgo, current, imgbase + glowInfoOffset, address);
    readIntMam(csgo, current, imgbase + glowInfoOffset + glowObjectLoopCount, count);
}

void localbaseInformation(mach_vm_address_t imgbase, int * i_teamNum){
    // read localbaseStartaDDress
    uint32_t localBase;
    readUint32Mam(csgo, current, imgbase + 0xED1854, &localBase);
    // read icrossHairID
    readUint32Mam(csgo, current, localBase + 0xe4, i_teamNum);
}


int main(int argc, const char * argv[]) {
    
    int pid = get_process("csgo_osx");
    printf("The pid is %i\n", pid);
    uint32_t * imgBase[2];
    const char * a[2] = {"/client.dylib", "/engine.dylib"};
    task_for_pid(current_task(), getpid(), &current);
    csgo = get_client_module_info(current_task(), current_task(), pid, imgBase, a, 2);
    task_for_pid(current_task(), getpid(), &current);
    clientBase = * imgBase[0];
    EngineBase = * imgBase[1];
    printf("Client start at 0x%x and engine start at 0x%x", clientBase, EngineBase);
    printf("csgo task is %i pid is %i current task is %i\n", csgo, pid, current);
    // collect info
    int iTeamNum;
    int glowObjectLoopCount;
    uint32_t glowObjectLoopStartAddress;
    localbaseInformation(clientBase, &iTeamNum);
    glowInfo(clientBase, &glowObjectLoopStartAddress, &glowObjectLoopCount);
    printf("glow loop address is 0x%x", glowObjectLoopStartAddress);
    // Apply Glow
    while (1) {
		localbaseInformation(clientBase, &iTeamNum);
        readPlayerPointAndHealth(clientBase, glowObjectLoopStartAddress, iTeamNum);
        //break;
        usleep(20000);
    }
    return 0;
}

