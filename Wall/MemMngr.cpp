//
//  MemMngr.cpp
//  Wall
//
//  Created by Andre Kalisch on 27.09.16.
//  Copyright © 2016 Andre Kalisch. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.


#include "MemMngr.hpp"

MemMngr::MemMngr(Process* g_cProc)
{
	this->g_cProc = g_cProc;
}

MemMngr::~MemMngr() {}

uint8_t* MemMngr::readData(vm_address_t moduleStart, size_t moduleSize)
{
	uint8_t* buffer = nullptr;
	vm_offset_t readMem;
	mach_msg_type_number_t sizeMax = (mach_msg_type_number_t)moduleSize;
	vm_read(g_cProc->mainTask(), (vm_address_t)moduleStart, sizeMax, &readMem, &sizeMax);
	uint64_t address = (uint64_t)readMem;
	buffer = (uint8_t*)address;
	return buffer;
}

std::string MemMngr::readString(mach_vm_address_t address)
{
	std::string str;
	Temp_t temp = read<Temp_t>(address);
	for (int i = 0; i < sizeof(Temp_t); ++i){
		if (temp.buffer[i] == '\0')
			break;
		str += temp.buffer[i];
	};
	return str;
}
