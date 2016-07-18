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

#include <iostream>
#include "Utils.hpp"
#include "Utils.cpp"
#include <ApplicationServices/ApplicationServices.h>


uint64_t glowInfoOffset = 0x594A900;
uint64_t LocalPlayerBase = 0x513B038;
uint64_t playerBase = 0x50B7678;


int m_iGlowIndex = 0xA2E0;
bool statBool = true;

bool ctr = false;
bool sft = false;
bool cmd = false;
bool opt = false;
bool states = true;

struct Color {
    float red;
    float green;
    float blue;
    float alpha;
};

CGEventRef keyBoardCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp) && (type != kCGEventFlagsChanged))
        return event;
    // The incoming keycode.
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    //    printf("%i", keycode);
    if(keycode == (CGKeyCode)59||keycode == (CGKeyCode)62){
        if(ctr){
            ctr = false;
        }
        else{
            ctr = true;
        }
    }
    if(ctr){
        CGEventSetFlags(event,(CGEventFlags)(NX_CONTROLMASK|CGEventGetFlags(event)));
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
        CGEventSetFlags(event,(CGEventFlags)(NX_SHIFTMASK|CGEventGetFlags(event)));
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
        CGEventSetFlags(event,(CGEventFlags)(NX_COMMANDMASK|CGEventGetFlags(event)));
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
        CGEventSetFlags(event,(CGEventFlags)(NX_ALTERNATEMASK|CGEventGetFlags(event)));
    }
    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t)keycode);
    if (keycode == 9) {
        states = !states;
        printf("State Changed! %i\n", states);
    }
    // We must return the event for it to be useful.
    return event;
}


void keyBoardListen(){
    CFMachPortRef	  eventTap;
    CGEventMask		eventMask;
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


int get_process(){
    pid_t pids[1024];
    int numberOfProcesses = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    for (int i = 0; i < numberOfProcesses; ++i) {
        if (pids[i] == 0) { continue; }
        char name[1024];
        proc_name(pids[i], name, sizeof(name));
        if (!strncmp(name, "csgo_osx64", sizeof("csgo_osx64"))) {
            return pids[i];
        }
    }
    return -1;
}

int get_client_module_info(task_t task, pid_t pid, mach_vm_address_t * start, unsigned long * length){
    kern_return_t error = task_for_pid(current_task(), pid, &task);
    printf("%d -> %x [%d - %s]\n", pid, task, error, mach_error_string(error));
    struct task_dyld_info dyld_info;
    mach_vm_address_t address;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    if (task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) == KERN_SUCCESS)
    {
        address = dyld_info.all_image_info_addr;
    }else{
        printf("failed\n");
        return 0;
    }
    printf("task is %i\n", task);
    struct dyld_all_image_infos *dyldaii;
    mach_msg_type_number_t size = sizeof(dyld_all_image_infos);
    vm_offset_t readMem;
    kern_return_t kr = vm_read(task, address, size, &readMem, &size);
    if (kr != KERN_SUCCESS) {
        printf("faild");
        return 0;
    }else{
        printf("successed~~");
        printf("%u \n", readMem);
    }
    dyldaii = (dyld_all_image_infos *) readMem;
    printf ("Version: %d, %d images at offset %p\n",
            dyldaii->version, dyldaii->infoArrayCount, dyldaii->infoArray);
    int imageCount = dyldaii->infoArrayCount;
    mach_msg_type_number_t dataCnt = imageCount * 24;
    struct dyld_image_info *g_dii = NULL;
    g_dii = (struct dyld_image_info *) malloc (dataCnt);
    // 32bit bs 64bit
    kern_return_t kr2 = vm_read(task, (mach_vm_address_t)dyldaii->infoArray, dataCnt, &readMem, &dataCnt);
    if (kr2) {
        printf("get lists failed \n");
        return 0;
    }else{
        
    }
    struct dyld_image_info *dii = (struct dyld_image_info *) readMem;
    for (int i = 0; i < imageCount; i++) {
        dataCnt = 1024;
        kern_return_t kr3 = vm_read(task, (mach_vm_address_t)dii[i].imageFilePath, dataCnt, &readMem, &dataCnt);
        char *imageName = (char *)readMem;
        if (imageName){
            g_dii[i].imageFilePath = strdup(imageName);
        }
        else{
            g_dii[i].imageFilePath = NULL;
        }
        g_dii[i].imageLoadAddress = dii[i].imageLoadAddress;
        if (strstr(imageName, "/client.dylib") != NULL ){
            *start = (mach_vm_address_t)dii[i].imageLoadAddress;
            printf("!!!!!!!!find %s\n, address is 0x%llx \n", imageName, (mach_vm_address_t)dii[i].imageLoadAddress);
        }
        if (strstr(imageName, "/engine.dylib") != NULL ){
            *start = (mach_vm_address_t)dii[i].imageLoadAddress;
            printf("!!!!!!!!find %s\n, address is 0x%llx \n", imageName, (mach_vm_address_t)dii[i].imageLoadAddress);
        }
    }
    return task;
}
template <typename Type>
int readRam(task_t task, uint64_t address, Type * result) {
    int * size = (int* )malloc(sizeof(int));
    *size = 4;
    uint32_t * sz = (uint32_t *)malloc(sizeof(uint32_t));
    vm_offset_t * data = (vm_offset_t *)malloc(sizeof(uint64_t));
    //vm_offset_t data;
    if (vm_read(task, address, *size, data, sz) != KERN_SUCCESS){
        free(sz);
        free(data);
        free(size);
        return 0;
    }
    *result = (Type) *(Type* )(*data);
    free(sz);
    free(size);
    free(data);
    return 1;
}

int getEntityGlowLoopStartAddressAndCount(task_t task, mach_vm_address_t imgbase, uint64_t * address, int * count){
    int glowObjectLoopCount = 0x18;
    auto reAddress = Utils::ReadMemAndDeAllocate<uint64_t>(task, current_task() ,imgbase + glowInfoOffset, address);
    auto reCount = readRam<int>(task, imgbase + glowInfoOffset + glowObjectLoopCount, count);
    if (!reAddress || !reCount) {
        return 0;
    }else{
        return 1;
    }
}

template <typename Type>
void WriteMem(task_t task, mach_vm_address_t address, Type value) {
    vm_write(task, address, (vm_offset_t) &value, sizeof(Type));
}

void applyGlowEffect(task_t task, mach_vm_address_t glowStartAddress, int glowObjectIndex, Color * color){
    WriteMem<bool>(task, glowStartAddress + 0x40 * glowObjectIndex + 0x28, statBool);
    WriteMem<float>(task, glowStartAddress + 0x40 * glowObjectIndex + 0x8, color->red);
    WriteMem<float>(task, glowStartAddress + 0x40 * glowObjectIndex + 0xc, color->green);
    WriteMem<float>(task, glowStartAddress + 0x40 * glowObjectIndex + 0x10, color->blue);
    WriteMem<float>(task, glowStartAddress + 0x40 * glowObjectIndex + 0x14, color->alpha);
}

void readPlayerPointAndHealth(task_t task, task_t taskSelf, mach_vm_address_t imgbase, mach_vm_address_t startAddress, int iTeamNum) {
    uint64_t memoryAddress;
    int glowIndex;
    //    printf("----------updated----------\n");
    for (int i = 0; i < 60; i++) {
        int playerTeamNum;
        if (Utils::ReadMemAndDeAllocate<uint64_t>(task, taskSelf, imgbase + playerBase + 0x20 * i, &memoryAddress) == -1) {
            continue;
        }
        if (memoryAddress <= 0x0){
            continue;
        }
        if (Utils::ReadMemAndDeAllocate<int>(task, taskSelf, memoryAddress + 0xac00, &glowIndex)) {
            continue;
        }
        int health;
        if (Utils::ReadMemAndDeAllocate<int>(task, taskSelf, memoryAddress + 0x12c, &health)) {
            continue;
        }
        if (Utils::ReadMemAndDeAllocate<int>(task, taskSelf, memoryAddress + 0x120, &playerTeamNum)) {
            continue;
        }
        if (playerTeamNum == iTeamNum || playerTeamNum == 0) {
            continue;
        }
        if (playerTeamNum == 0) {
            continue;
        }
        //        bool dormant;
        //        if (Utils::ReadMemAndDeAllocate(task, taskSelf, memoryAddress + 0xee, &dormant)) {
        //            if (dormant == 0) {
        //                continue;
        //            }
        //        }
        printf("The memory address read is 0x%x player %i health is %i iteam is %i playerteam is %i \n", memoryAddress, glowIndex, health, iTeamNum, playerTeamNum);
        if (health == 0){
            health = 100;
        }
        Color color = {float((100 - health) / 100.0), float((health) / 100.0), 0.0f, 0.8f};
        applyGlowEffect(task, startAddress, glowIndex, &color);
    }
}

int testLocalPlayerAddress(task_t csgo, uint64_t clientBase){
    uint64_t playerAddress;
    int health, iTeamNum;
    Utils::ReadMemAndDeAllocate(csgo, current_task(), clientBase + LocalPlayerBase, &playerAddress);
    Utils::ReadMemAndDeAllocate(csgo, current_task(), playerAddress + 0x12c, &health);
    Utils::ReadMemAndDeAllocate(csgo, current_task(), playerAddress + 0x120, &iTeamNum);
    printf("I team is %i and team %i\n", iTeamNum, health);
    return iTeamNum;
}


int main(int argc, const char * argv[]) {
    // keyboard listen
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_BACKGROUND, 0), ^{
        keyBoardListen();
    });
    unsigned long moduleLength = 0x0511e000;
    task_t task;
    mach_vm_address_t moduleStartAddress;
    pid_t pid = get_process();
    if (pid == -1) {
        printf("Cant get pid of csgo_osx");
        exit(0);
    }else{
        printf("the pid of csgo is %i\n", pid);
    }
    task = get_client_module_info(task, pid, &moduleStartAddress, &moduleLength);
    if (task){
        printf("Successfully get the Client.dylib, address is 0x%llx \n", moduleStartAddress);
        printf("Module should end at 0x%llx\n", moduleStartAddress + moduleLength);
    }else{
        printf("failed\n");
    }
    printf("task is %i\n", task);
    // info check end
    uint64_t glowObjectLoopStartAddress;
    int glowObjectLoopCount = 0;
    auto g = getEntityGlowLoopStartAddressAndCount(task, moduleStartAddress, &glowObjectLoopStartAddress, &glowObjectLoopCount);
    while(true){
        if (states){
            int i_teamNum = testLocalPlayerAddress(task, moduleStartAddress);
            readPlayerPointAndHealth(task, current_task(), moduleStartAddress, glowObjectLoopStartAddress, i_teamNum);
        }
        usleep(100);
    }
    
}
