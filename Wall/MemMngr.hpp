//
//  MemMngr.hpp
//
//  Created by Andre Kalisch on 27.09.16.
//  Copyright © 2016 Andre Kalisch. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.

#ifndef MemMngr_hpp
#define MemMngr_hpp

#include "Process.hpp"

#pragma once
pid_t mainPid = -1;
task_t mainTask = 0;

struct Temp_t{
	char buffer[256];
};

class MemMngr {
public:
    explicit MemMngr() {}
    
    ~MemMngr() {}
    
	template <typename type>
	type read(mach_vm_address_t address, size_t extraSize = sizeof(type), bool debug = false){
		if (debug)
			printf("read: 0x%lx\n", (vm_address_t)address);
		vm_offset_t data;
		uint32_t sz;
		auto re = vm_read(mainTask, (vm_address_t)address, extraSize, &data, &sz);
		if (re == KERN_SUCCESS){
			type content = (type)*((type*)data);
			vm_deallocate(current_task(), data, sz);
            return content;
		}
		return type();
	}

	uint8_t* readData(vm_address_t moduleStart, size_t moduleSize){
		uint8_t* buffer = nullptr;
		vm_offset_t readMem;
		mach_msg_type_number_t sizeMax = (mach_msg_type_number_t)moduleSize;
		vm_read(mainTask, (vm_address_t)moduleStart, sizeMax, &readMem, &sizeMax);
		uint64_t address = (uint64_t)readMem;
		buffer = (uint8_t*)address;
		return buffer;
	}

	template <typename type>
	bool write(mach_vm_address_t address, type data, bool debug = false){
		if (debug)
			printf("read: 0x%lx\n", (vm_address_t)address);
		if (address == 0x0)
			return false;
		auto re = vm_write(mainTask, address, (vm_offset_t)&data, sizeof(data));
		if (re == KERN_SUCCESS)
			return true;
		return false;
	}

	std::string readString(mach_vm_address_t address){
		std::string str;
		Temp_t temp = read<Temp_t>(address);
		for (int i = 0; i < sizeof(Temp_t); ++i){
			if (temp.buffer[i] == '\0')
				break;
			str += temp.buffer[i];
		};
		return str;
	}
	
};

#endif
