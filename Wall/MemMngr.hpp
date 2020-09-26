//
//  MemMngr.hpp
//  Wall
//
//  Created by Andre Kalisch on 27.09.16.
//  Copyright © 2016 Andre Kalisch. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.

#ifndef MEMMNGR_HPP
#define MEMMNGR_HPP

#include "Process.hpp"

class MemMngr {
	struct Temp_t {
		char buffer[256];
	};
	Process* g_cProc = nullptr;
public:
	explicit MemMngr(Process* g_cProc = nullptr);
	~MemMngr();
	
	template <typename type>
	type read(mach_vm_address_t address, size_t extraSize = sizeof(type), bool debug = false);
	uint8_t* readData(vm_address_t moduleStart, size_t moduleSize);
	
	template <typename type>
	bool write(mach_vm_address_t address, type data, bool debug = false);
	std::string readString(mach_vm_address_t address);
};

template <typename type>
type MemMngr::read(mach_vm_address_t address, size_t extraSize, bool debug)
{
	if (debug)
		printf("read: 0x%lx\n", (vm_address_t)address);
	vm_offset_t data;
	uint32_t sz;
	auto re = vm_read(g_cProc->mainTask(), (vm_address_t)address, extraSize, &data, &sz);
	if (re == KERN_SUCCESS){
		type content = (type)*((type*)data);
		vm_deallocate(current_task(), data, sz);
		return content;
	}
	return type();
}

template <typename type>
bool MemMngr::write(mach_vm_address_t address, type data, bool debug)
{
	if (debug)
		printf("read: 0x%lx\n", (vm_address_t)address);
	if (address == 0x0)
		return false;
	auto re = vm_write(g_cProc->mainTask(), address, (vm_offset_t)&data, sizeof(data));
	if (re == KERN_SUCCESS)
		return true;
	return false;
}

#endif
