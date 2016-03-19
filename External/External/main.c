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
#include <ApplicationServices/ApplicationServices.h>


task_t csgo, current;
uint32_t clientBase, engineBase;
struct Color {
    float red;
    float blue;
    float green;
    float alpha;
};

//Global Variables to keey track of modifier keys pressed
bool ctr = false;
bool sft = false;
bool cmd = false;
bool opt = false;
bool states = true;
bool uArrow = false;
bool dArrow = false;
int playTeam = 0;

// This callback will be invoked every time there is a keystroke.
//
CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    //printf("called!\n");
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp) && (type != kCGEventFlagsChanged))
        return event;
    
    // The incoming keycode.
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    //Control
    if(keycode == (CGKeyCode)59||keycode == (CGKeyCode)62){
        if(ctr){
            ctr = false;
        }
        else{
            ctr = true;
        }
    }
    if(ctr){
        CGEventSetFlags(event,NX_CONTROLMASK|CGEventGetFlags(event));
    }
    //Shift
    if(keycode == (CGKeyCode)60||keycode == (CGKeyCode)56){
        if(sft){
            sft = false;
        }
        else{
            sft = true;
        }
    }
    if(sft){
        CGEventSetFlags(event,NX_SHIFTMASK|CGEventGetFlags(event));
    }
    //Command
    if(keycode == (CGKeyCode)55||keycode == (CGKeyCode)54){
        if(cmd){
            cmd = false;
        }
        else{
            cmd = true;
        }
    }
    if(cmd){
        CGEventSetFlags(event,NX_COMMANDMASK|CGEventGetFlags(event));
    }
    //Option
    if(keycode == (CGKeyCode)58||keycode == (CGKeyCode)61){
        if(opt){
            opt = false;
        }
        else{
            opt = true;
        }
    }
    if(opt){
        CGEventSetFlags(event,NX_ALTERNATEMASK|CGEventGetFlags(event));
    }
    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t)keycode);
    if (cmd && sft && keycode == 7) {
        states = !states;
        printf("now states is %i\n", states);
    }
    //Team T
    if(keycode == (CGKeyCode)125||keycode == (CGKeyCode)108)
    {
        if(dArrow){
            dArrow = false;
        }
        else{
            dArrow = true;
        }
        if(dArrow){
            playTeam = 2;
            CGEventSetFlags(event,NX_DOWN_ARROW_KEY|CGEventGetFlags(event));
        }
    }
    //Team CT
    if(keycode == (CGKeyCode)126||keycode == (CGKeyCode)103)
    {
        if(uArrow){
            uArrow = false;
        }
        else{
            uArrow = true;
        }
    if(uArrow){
        playTeam = 3;
        CGEventSetFlags(event,NX_UP_ARROW_KEY|CGEventGetFlags(event));
        }
    }
    // We must return the event for it to be useful.
    return event;
}



void startListen(){
    CFMachPortRef	  eventTap;
    CGEventMask		eventMask;
    CFRunLoopSourceRef runLoopSource;
    
    // Create an event tap. We are interested in key presses.
    //eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp) | (1 << kCGEventFlagsChanged));
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventFlagsChanged));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
                                eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }
    
    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(
                                                  kCFAllocatorDefault, eventTap, 0);
    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
                       kCFRunLoopCommonModes);
    
    // Enable the event tap.
    CGEventTapEnable(eventTap, true);
    
    // Set it all running.
    CFRunLoopRun();
    
    // In a real program, one would have arranged for cleaning up.
}


void applyGlowEffect(uint32_t glowStartAddress, int glowObjectIndex, struct Color * color){
    bool stat = 1;
    printf("glow state is %i, alpha is %f\n", states, color->alpha);
	vm_write(csgo, glowStartAddress + 0x38 * glowObjectIndex + 0x24, (vm_offset_t) &stat, sizeof(bool));
    vm_write(csgo, glowStartAddress + 0x38 * glowObjectIndex + 0x4, (vm_offset_t) &(color->red), sizeof(struct Color));
}

void readPlayerPointAndHealth(mach_vm_address_t imgbase, uint32_t startAddress, int iTeamNum) {
    int m_iGlowIndex = 0xA2D0;
    uint32_t memoryAddress;
    int glowIndex;
    int playerBase = 0xEE5914;
    printf("----------updated----------\n");
    for (int i = 0; i < 0x60; i++) {
        if (readIntMam(csgo, current, imgbase + playerBase + 0x10 * i, &memoryAddress) == -1)//Entetiy address
            continue;
        if (memoryAddress == 0x0)//is nullpointer
            continue;
        if (readIntMam(csgo, current, memoryAddress + 0xA2E0, &glowIndex)) //Read GlowIndex
            continue;
        int health;
        if (readIntMam(csgo, current, memoryAddress + 0xf0, &health)) //Read Health
            continue;
		if(health < 1)
			continue;
		int playerTeamNum;
        if (readIntMam(csgo, current, memoryAddress + 0xE4, &playerTeamNum)) //Read Team Number
            continue;
      //Reassign playerTeamNum to Up or Down Arrow Key
        iTeamNum = playTeam;
//        iTeamNum = playTeam;
        if (playerTeamNum == 0 || playerTeamNum == iTeamNum || playerTeamNum == playTeam)
            continue;
/*        
    if (playerTeamNum == 0 || playerTeamNum == iTeamNum || playerTeamNum == 0)
            continue;
 */
        printf("Read player %i health is %i team is %i, glow index is %i\n", i, health, playerTeamNum, glowIndex);
        struct Color color;
        color.red = (100 - health) / 100.0;
        color.blue = 0.0f;
        color.green = (health) / 100.0;
        color.alpha = 0.8f;
        applyGlowEffect(startAddress, glowIndex, &color);
    }
}

void glowInfo(mach_vm_address_t imgbase, uint32_t * address){
    int glowInfoOffset = 0x1344050;
    readUint32Mam(csgo, current, imgbase + glowInfoOffset, address);
}

void localbaseInformation(mach_vm_address_t imgbase, int * i_teamNum){
    // read localbaseStartaDDress
    uint32_t localBase;
    readUint32Mam(csgo, current, imgbase + 0xEE5904 + 0x10, &localBase);
    // read icrossHairID
    readUint32Mam(csgo, current, localBase + 0xe4, i_teamNum);
}


int main(int argc, const char * argv[]) {
    
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_BACKGROUND, 0), ^{
        startListen();
    });
    int pid = get_process("csgo_osx");
    printf("The pid is %i\n", pid);
    uint32_t * imgBase[2];
    const char * a[2] = {"/client.dylib", "/engine.dylib"};
    task_for_pid(current_task(), getpid(), &current);
    csgo = get_client_module_info(current_task(), current_task(), pid, imgBase, a, 2);
    if (csgo == -1) {
        printf("No root permission\nPlease run it with root\n");
        exit(0);
    }
    task_for_pid(current_task(), getpid(), &current);
    clientBase = * imgBase;
    engineBase = * (imgBase + 1);

    // collect info
    int iTeamNum;
    uint32_t glowObjectLoopStartAddress;
    localbaseInformation(clientBase, &iTeamNum);
    glowInfo(clientBase, &glowObjectLoopStartAddress);
    printf("glow loop address is 0x%x", glowObjectLoopStartAddress);
    // Apply Glow
    while (1) {
        if (states) {
            localbaseInformation(clientBase, &iTeamNum);
            readPlayerPointAndHealth(clientBase, glowObjectLoopStartAddress, iTeamNum);
        }
        //break;
        usleep(20000);
    }
    return 0;
}

