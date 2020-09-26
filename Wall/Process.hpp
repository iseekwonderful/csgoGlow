//
//  Process.hpp
//  Wall
//
//  Created by Andre Kalisch on 30.08.16.
//  Copyright © 2016 aKalisch. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ApplicationServices/ApplicationServices.h>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach.h>
#include <mach-o/dyld_images.h>
#include <mach-o/loader.h>
#include <libproc.h>
#include <sys/stat.h>

class Process {
	pid_t mainPid_ = -1;
	task_t mainTask_ = 0;
public:
	explicit Process();
	~Process();
	
	pid_t& mainPid();
	task_t& mainTask();
    
	int get(const char* procname);
	int task(pid_t pid);
	void getModule(task_t task, mach_vm_address_t* start, off_t* length, const char* module, Byte* buffer = nullptr, bool readBuffer = false);
};

#endif
