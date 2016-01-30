//
//  Utils.c
//  External
//
//  Created by XuYang on 16/1/30.
//  Copyright © 2016年 nadp.me. All rights reserved.
//

#include <libproc.h>
#include "Utils.h"
#include <string.h>
#include <mach/mach.h>
#include <mach-o/dyld_images.h>
#include <unistd.h>


int readIntMam(task_t target, task_t self, mach_vm_address_t address, int * content) {
    vm_offset_t data;
    uint32_t sz;
    kern_return_t re = vm_read(target, address, sizeof(int), &data, &sz);
    if (re != 0){
        return -1;
    }
    *content = (int) *(int *)(data);
    // release ram
    vm_deallocate(self, data, sz);
    return 0;
}

int readBoolMam(task_t target, task_t self, mach_vm_address_t address, int8_t * content) {
    vm_offset_t data;
    uint32_t sz;
    kern_return_t re = vm_read(target, address, sizeof(int8_t), &data, &sz);
    if (re != 0){
        return -1;
    }
    *content = (int8_t) *(int8_t *)(data);
    // release ram
    vm_deallocate(self, data, sz);
    return 0;
}


int readUint32Mam(task_t target, task_t self, mach_vm_address_t address, uint32_t * content) {
    vm_offset_t data;
    uint32_t sz;
    kern_return_t re = vm_read(target, address, sizeof(uint32_t), &data, &sz);
    if (re != 0){
        return -1;
    }
    *content = (uint32_t) *(uint32_t *)(data);
    // release ram
    vm_deallocate(self, data, sz);
    return 0;
}

int readFloatMam(task_t target, task_t self, mach_vm_address_t address, float * content) {
    vm_offset_t data;
    uint32_t sz;
    kern_return_t re = vm_read(target, address, sizeof(float), &data, &sz);
    if (re != 0){
        return -1;
    }
    *content = (float) *(float *)(data);
    // release ram
    vm_deallocate(self, data, sz);
    return 0;
}

int get_process(char * processName){
    // get pid of csgo
    pid_t pids[1024];
    int numberOfProcesses = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    for (int i = 0; i < numberOfProcesses; ++i) {
        if (pids[i] == 0) { continue; }
        char name[1024];
        proc_name(pids[i], name, sizeof(name));
        if (!strncmp(name, processName, strlen(processName))) {
            return pids[i];
        }
    }
    return -1;
}

int get_client_module_info(task_t task, task_t current, pid_t pid, uint32_t * start[], const char * query[], int queryLength){
    kern_return_t error = task_for_pid(current_task(), pid, &task);
    if (error != KERN_SUCCESS) {
        fprintf(stderr, "[Gaben:WTF]\n");
        return 0;
    }
    struct task_dyld_info dyld_info;
    mach_vm_address_t address;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    if (task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) == KERN_SUCCESS)
    {
        address = dyld_info.all_image_info_addr;
    }else{
        fprintf(stderr ,"failed\n");
        return 0;
    }
    struct dyld_all_image_infos *dyldaii;
    mach_msg_type_number_t size = sizeof(struct dyld_all_image_infos);
    vm_offset_t readMem;
    kern_return_t kr = vm_read(task, address, size, &readMem, &size);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "faild");
        return 0;
    }
    dyldaii = (struct dyld_all_image_infos *) readMem;
    printf ("Version: %d, %d images at offset %p\n",
            dyldaii->version, dyldaii->infoArrayCount, dyldaii->infoArray);
    int imageCount = dyldaii->infoArrayCount;
    mach_msg_type_number_t dataCnt = imageCount * 24;
    struct dyld_image_info *g_dii = NULL;
    g_dii = (struct dyld_image_info *) malloc (dataCnt);
    kern_return_t kr2 = vm_read(task, (mach_vm_address_t)dyldaii->infoArray, dataCnt, &readMem, &dataCnt);
    if (kr2) {
        printf("get lists failed \n");
        return 0;
    }
    struct dyld_image_info *dii = (struct dyld_image_info *) readMem;
    for (int i = 0; i < imageCount; i++) {
        dataCnt = 1024;
        vm_read(task, (mach_vm_address_t)dii[i].imageFilePath, dataCnt, &readMem, &dataCnt);
        char *imageName = (char *)readMem;
        if (imageName){
            g_dii[i].imageFilePath = strdup(imageName);
        }
        else{
            g_dii[i].imageFilePath = NULL;
        }
        g_dii[i].imageLoadAddress = dii[i].imageLoadAddress;
        for (int j = 0; j < queryLength; j++) {
            if (strstr(imageName, query[j]) != NULL ){
                printf("[Gaben:] I tell U %s\n, address is 0x%llx \n", imageName, (mach_vm_address_t)dii[i].imageLoadAddress);
                start[j] = (mach_vm_address_t)dii[i].imageLoadAddress;
            }
        }
    }
    return task;
}