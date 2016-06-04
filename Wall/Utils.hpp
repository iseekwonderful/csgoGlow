//
//  Utils.hpp
//  Wall
//
//  Created by sheep on 16/5/14.
//  Copyright © 2016年 nadp. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <stdlib.h>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach.h>
#include <mach-o/dyld_images.h>
#include <libproc.h>
#include <string>


class Utils {
    
public:
    // Function That Read And Write Memory
    template <typename type>
    static int ReadMemAndDeAllocate(task_t target, task_t self, mach_vm_address_t address, type * content);
    
    template <typename type>
    static int WriteMemAndDeAllocate(task_t target, mach_vm_address_t address, type data);
    
    //static const char * readStringMemoryAndAllocate(task_t target, task_t self, mach_vm_address_t address);
    static const char * readStringMemoryAndAllocate(task_t target, task_t self, mach_vm_address_t address){
        std::string strings;
        char aaa;
        mach_msg_type_number_t dataCntu;
        vm_offset_t offsetu;
        vm_read(target, (mach_vm_address_t)address, sizeof(char), &offsetu, &dataCntu);
        aaa = (char) *(uint32_t *)(offsetu);
        strings += aaa;
        int count = 1;
        while (aaa != '\0'){
            vm_read(target, (mach_vm_address_t)(address + count), sizeof(char), &offsetu, &dataCntu);
            aaa = (char) *(uint32_t *)(offsetu);
            strings += aaa;
            count ++;
        }
        return strings.c_str();
    }
    
    // define some offset
};

#endif /* Utils_hpp */
