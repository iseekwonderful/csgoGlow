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

pid_t   mainPid     = g_cProc->get("csgo_osx64");
task_t  mainTask    = g_cProc->task(mainPid);

#include "memory/manager.cpp"
#include "memory/scanner.cpp"

uint64_t glowInfoOffset;
uint64_t LocalPlayerBase;
uint64_t playerBase;

uint64_t m_iGlowIndex       = 0xAC10;
bool statBool = true;

bool ctr = false;
bool sft = false;
bool cmd = false;
bool alt = false;
bool states = true;

struct Color {
    float red;
    float green;
    float blue;
    float alpha;
};

uint16_t keyCodeForKeyString(const char * keyString) {
    if (strcmp(keyString, "a") == 0) return 0;
    if (strcmp(keyString, "s") == 0) return 1;
    if (strcmp(keyString, "d") == 0) return 2;
    if (strcmp(keyString, "f") == 0) return 3;
    if (strcmp(keyString, "h") == 0) return 4;
    if (strcmp(keyString, "g") == 0) return 5;
    if (strcmp(keyString, "z") == 0) return 6;
    if (strcmp(keyString, "x") == 0) return 7;
    if (strcmp(keyString, "c") == 0) return 8;
    if (strcmp(keyString, "v") == 0) return 9;
    if (strcmp(keyString, "b") == 0) return 11;
    if (strcmp(keyString, "q") == 0) return 12;
    if (strcmp(keyString, "w") == 0) return 13;
    if (strcmp(keyString, "e") == 0) return 14;
    if (strcmp(keyString, "r") == 0) return 15;
    if (strcmp(keyString, "y") == 0) return 16;
    if (strcmp(keyString, "t") == 0) return 17;
    if (strcmp(keyString, "1") == 0) return 18;
    if (strcmp(keyString, "2") == 0) return 19;
    if (strcmp(keyString, "3") == 0) return 20;
    if (strcmp(keyString, "4") == 0) return 21;
    if (strcmp(keyString, "6") == 0) return 22;
    if (strcmp(keyString, "5") == 0) return 23;
    if (strcmp(keyString, "=") == 0) return 24;
    if (strcmp(keyString, "9") == 0) return 25;
    if (strcmp(keyString, "7") == 0) return 26;
    if (strcmp(keyString, "-") == 0) return 27;
    if (strcmp(keyString, "8") == 0) return 28;
    if (strcmp(keyString, "0") == 0) return 29;
    if (strcmp(keyString, "]") == 0) return 30;
    if (strcmp(keyString, "o") == 0) return 31;
    if (strcmp(keyString, "u") == 0) return 32;
    if (strcmp(keyString, "[") == 0) return 33;
    if (strcmp(keyString, "i") == 0) return 34;
    if (strcmp(keyString, "p") == 0) return 35;
    if (strcmp(keyString, "RETURN") == 0) return 36;
    if (strcmp(keyString, "l") == 0) return 37;
    if (strcmp(keyString, "j") == 0) return 38;
    if (strcmp(keyString, "'") == 0) return 39;
    if (strcmp(keyString, "k") == 0) return 40;
    if (strcmp(keyString, ";") == 0) return 41;
    if (strcmp(keyString, "\\") == 0) return 42;
    if (strcmp(keyString, ",") == 0) return 43;
    if (strcmp(keyString, "/") == 0) return 44;
    if (strcmp(keyString, "n") == 0) return 45;
    if (strcmp(keyString, "m") == 0) return 46;
    if (strcmp(keyString, ".") == 0) return 47;
    if (strcmp(keyString, "TAB") == 0) return 48;
    if (strcmp(keyString, "SPACE") == 0) return 49;
    if (strcmp(keyString, "`") == 0) return 50;
    if (strcmp(keyString, "DELETE") == 0) return 51;
    if (strcmp(keyString, "ENTER") == 0) return 52;
    if (strcmp(keyString, "ESCAPE") == 0) return 53;
    if (strcmp(keyString, ".") == 0) return 65;
    if (strcmp(keyString, "*") == 0) return 67;
    if (strcmp(keyString, "+") == 0) return 69;
    if (strcmp(keyString, "CLEAR") == 0) return 71;
    if (strcmp(keyString, "/") == 0) return 75;
    if (strcmp(keyString, "ENTER") == 0) return 76;
    if (strcmp(keyString, "=") == 0) return 78;
    if (strcmp(keyString, "=") == 0) return 81;
    if (strcmp(keyString, "0") == 0) return 82;
    if (strcmp(keyString, "1") == 0) return 83;
    if (strcmp(keyString, "2") == 0) return 84;
    if (strcmp(keyString, "3") == 0) return 85;
    if (strcmp(keyString, "4") == 0) return 86;
    if (strcmp(keyString, "5") == 0) return 87;
    if (strcmp(keyString, "6") == 0) return 88;
    if (strcmp(keyString, "7") == 0) return 89;
    if (strcmp(keyString, "8") == 0) return 91;
    if (strcmp(keyString, "9") == 0) return 92;
    if (strcmp(keyString, "F5") == 0) return 96;
    if (strcmp(keyString, "F6") == 0) return 97;
    if (strcmp(keyString, "F7") == 0) return 98;
    if (strcmp(keyString, "F3") == 0) return 99;
    if (strcmp(keyString, "F8") == 0) return 100;
    if (strcmp(keyString, "F9") == 0) return 101;
    if (strcmp(keyString, "F11") == 0) return 103;
    if (strcmp(keyString, "F13") == 0) return 105;
    if (strcmp(keyString, "F14") == 0) return 107;
    if (strcmp(keyString, "F10") == 0) return 109;
    if (strcmp(keyString, "F12") == 0) return 111;
    if (strcmp(keyString, "F15") == 0) return 113;
    if (strcmp(keyString, "HELP") == 0) return 114;
    if (strcmp(keyString, "HOME") == 0) return 115;
    if (strcmp(keyString, "PGUP") == 0) return 116;
    if (strcmp(keyString, "DELETE") == 0) return 117;
    if (strcmp(keyString, "F4") == 0) return 118;
    if (strcmp(keyString, "END") == 0) return 119;
    if (strcmp(keyString, "F2") == 0) return 120;
    if (strcmp(keyString, "PGDN") == 0) return 121;
    if (strcmp(keyString, "F1") == 0) return 122;
    if (strcmp(keyString, "LEFT") == 0) return 123;
    if (strcmp(keyString, "RIGHT") == 0) return 124;
    if (strcmp(keyString, "DOWN") == 0) return 125;
    if (strcmp(keyString, "UP") == 0) return 126;
    
    fprintf(stderr, "keyString %s Not Found. Aborting...\n", keyString);
    exit(EXIT_FAILURE);
}

CGEventRef keyBoardCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp) && (type != kCGEventFlagsChanged)) return event;
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (keycode == (CGKeyCode)59 || keycode == (CGKeyCode)62) {
        if (ctr) {
            ctr = false;
        } else {
            ctr = true;
        }
    }
    if (ctr) {
        CGEventSetFlags(event, (CGEventFlags)(NX_CONTROLMASK | CGEventGetFlags(event)));
    }

    if (keycode == (CGKeyCode)60 || keycode == (CGKeyCode)56) {
        if (sft) {
            sft = false;
        } else {
            sft = true;
        }
    }
    if (sft) {
        CGEventSetFlags(event, (CGEventFlags)(NX_SHIFTMASK | CGEventGetFlags(event)));
    }
    
    if (keycode == (CGKeyCode)55 || keycode == (CGKeyCode)54) {
        if (cmd) {
            cmd = false;
        } else {
            cmd = true;
        }
    }
    if (cmd) {
        CGEventSetFlags(event, (CGEventFlags)(NX_COMMANDMASK | CGEventGetFlags(event)));
    }

    if (keycode == (CGKeyCode)58 || keycode == (CGKeyCode)61) {
        if (alt) {
            alt = false;
        } else{
            alt = true;
        }
    }
    if (alt) {
        CGEventSetFlags(event, (CGEventFlags)(NX_ALTERNATEMASK | CGEventGetFlags(event)));
    }
    
    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t)keycode);
    if (ctr && alt) {
        if (keycode == keyCodeForKeyString("v")) {
            states = !states;
            printf("State Changed! %s\n", states ? "ON" : "OFF");
        }
    }

    return event;
}

void keyBoardListen() {
    CFMachPortRef     eventTap;
    CGEventMask     eventMask;
    CFRunLoopSourceRef runLoopSource;
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventFlagsChanged));
    eventTap = CGEventTapCreate((CGEventTapLocation)kCGSessionEventTap, (CGEventTapPlacement)kCGHeadInsertEventTap, (CGEventTapOptions)0, eventMask, keyBoardCallback, NULL);
    if (!eventTap) {
        exit(1);
    }
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
}

void getEntityGlowLoopStartAddressAndCount(task_t task, mach_vm_address_t imgbase, uint64_t * address, int * count) {
    int glowObjectLoopCount = 0x18;
    uint64_t glowAddress = mem->read<uint64_t>(imgbase + glowInfoOffset);
    *address = glowAddress;
    int glowCount   = mem->read<int>(imgbase + glowInfoOffset + glowObjectLoopCount);
    *count = glowCount;
}

void applyGlowEffect(task_t task, mach_vm_address_t glowStartAddress, int glowObjectIndex, Color color) {
    uint64_t glowBaseAddress = glowStartAddress + (0x40 * glowObjectIndex);
    
    mem->write<bool>(glowBaseAddress + 0x28, statBool);
    mem->write<Color>(glowBaseAddress + 0x8, color);
}

void readPlayerPointAndHealth(mach_vm_address_t imgbase, mach_vm_address_t startAddress, int iTeamNum) {
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
        applyGlowEffect(mainTask, startAddress, glowIndex, color);
    }
}

int testLocalPlayerAddress(uint64_t clientBase) {
    uint64_t playerAddress  = mem->read<uint64_t>(clientBase + LocalPlayerBase);
    // Developer Comment:
    // int health              = mem->read<int>(playerAddress + 0x134);
    // We don't read this because it was never been used
    int iTeamNum            = mem->read<int>(playerAddress + 0x128);

    return iTeamNum;
}

int main(int argc, const char * argv[]) {
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_BACKGROUND, 0), ^{
        keyBoardListen();
    });

    if (mainPid == -1) {
        printf("Cant find the PID of CSGO\n");
        exit(0);
    } else {
        printf("Found CSGO PID: %i\n", mainPid);
    }
    
    off_t moduleLength = 0;
    mach_vm_address_t moduleStartAddress;
    g_cProc->getModule(mainTask, &moduleStartAddress, &moduleLength, "/client.dylib");
    
    Scanner * clientScanner = new Scanner(moduleStartAddress, moduleLength);
    
    LocalPlayerBase = clientScanner->getPointer(
        (Byte*)"\x89\xD6\x41\x89\x00\x49\x89\x00\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x00",
        "xxxx?xx?xxx????xxxx?",
        0xB
    ) + 0x4;
    
    playerBase = clientScanner->getPointer(
        (Byte*)"\x48\x8D\x1D\x00\x00\x00\x00\x48\x89\x00\xE8\x28\xD6\x00\x00",
        "xxx????xx?xxx??",
        0x3
    ) + 0x2C;

    glowInfoOffset = clientScanner->getPointer(
        (Byte*)"\x48\x8D\x3D\x00\x00\x00\x05\xE8\x00\x00\x00\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\xC7\x00\x00\x00\x00\x00\x00\x00\x00\x00\x48\x8D\x00\x00\x00\x00\x00",
        "xxx???xx????xxxx????xx?????????xx?????",
        0x22
    ) + 0x4;

    if (mainTask) {
        printf("Found the Client.dylib address: 0x%llx \n", moduleStartAddress);
        printf("Module should end at 0x%llx\n", moduleStartAddress + moduleLength);
    } else {
        printf("Failed to get the Client.dylib address\n");
    }
    
    uint64_t glowObjectLoopStartAddress;
    int glowObjectLoopCount = 0;
    getEntityGlowLoopStartAddressAndCount(mainTask, moduleStartAddress, &glowObjectLoopStartAddress, &glowObjectLoopCount);
    while (true)
    {
        if (states)
        {
            int i_teamNum = testLocalPlayerAddress(moduleStartAddress);
            readPlayerPointAndHealth(moduleStartAddress, glowObjectLoopStartAddress, i_teamNum);
        }
        usleep(7800);
    }
    
}
