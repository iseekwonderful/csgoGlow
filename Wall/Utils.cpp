//
//  Utils.cpp
//  Wall
//
//  Created by sheep on 16/5/14.
//  Copyright © 2016年 nadp. All rights reserved.
//

#include "Utils.hpp"


template <typename type>
int Utils::ReadMemAndDeAllocate(task_t target, task_t self, mach_vm_address_t address, type * content) {
    vm_offset_t data;
    uint32_t sz;
    auto re = vm_read(target, address, sizeof(type), &data, &sz);
    if (re != 0){
        return -1;
    }
    *content = (type) *(type *)(data);
    // release ram
    vm_deallocate(self, data, sz);
    return 0;
}

template <typename type>
int Utils::WriteMemAndDeAllocate(task_t target, mach_vm_address_t address, type data) {
    auto re = vm_write(target, address, (vm_offset_t)&data, sizeof(data));
    if (re != KERN_SUCCESS){
        return 0;
    }else{
        return 1;
    }
}


