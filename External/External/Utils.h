//
//  Utils.h
//  External
//
//  Created by XuYang on 16/1/30.
//  Copyright © 2016年 nadp.me. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <stdio.h>
#include <mach/mach.h>

int readBoolMam(task_t target, task_t self, mach_vm_address_t address, int8_t * content);
int readIntMam(task_t target, task_t self, mach_vm_address_t address, int * content);
int readFloatMam(task_t target, task_t self, mach_vm_address_t address, float * content);
int readUint32Mam(task_t target, task_t self, mach_vm_address_t address, uint32_t * content);
int get_process(char * processName);
int get_client_module_info(task_t task, task_t current, pid_t pid, uint32_t * start[], const char * query[], int queryLength);

#endif /* Utils_h */
