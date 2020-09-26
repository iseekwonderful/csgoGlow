//
//  Process.cpp
//  Wall
//
//  Created by Andre Kalisch on 30.08.16.
//  Copyright © 2016 aKalisch. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.


#include "Process.hpp"

Process::Process() {}

Process::~Process() {}

pid_t& Process::mainPid()
{
	return mainPid_;
}

task_t& Process::mainTask()
{
	return mainTask_;
}

int Process::get(const char* procname)
{
	int numberOfProcesses = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
	pid_t pids[numberOfProcesses];
	proc_listpids(PROC_ALL_PIDS, 0, pids, (int)sizeof(pids));
	for (int i = 0; i < numberOfProcesses; ++i){
		if (pids[i] == 0)
			continue;
		char name[1024];
		proc_name(pids[i], name, sizeof(name));
		if (!strncmp(name, (const char*)procname, sizeof(procname)))
			return pids[i];
	}
	return -1;
}

int Process::task(pid_t pid)
{
	task_t task = 0;
	task_for_pid(current_task(), pid, &task);
	return task;
}

void Process::getModule(task_t task, mach_vm_address_t* start, off_t* length, const char* module, Byte* buffer, bool readBuffer)
{
	struct task_dyld_info dyld_info;
	mach_vm_address_t address = 0;
	mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
	if (task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) == KERN_SUCCESS)
		address = dyld_info.all_image_info_addr;
	struct dyld_all_image_infos* dyldaii = nullptr;
	mach_msg_type_number_t size = sizeof(dyld_all_image_infos);
	vm_offset_t readMem;
	kern_return_t kr = vm_read(task, address, size, &readMem, &size);
	if (kr != KERN_SUCCESS)
		return;
	dyldaii = (dyld_all_image_infos*) readMem;
	int imageCount = dyldaii->infoArrayCount;
	mach_msg_type_number_t dataCnt = imageCount * 24;
	struct dyld_image_info* g_dii = new struct dyld_image_info[dataCnt];
	kern_return_t kr2 = vm_read(task, (mach_vm_address_t)dyldaii->infoArray, dataCnt, &readMem, &dataCnt);
	if (kr2) {
		delete[] g_dii;
		return;
	}
	struct dyld_image_info *dii = (struct dyld_image_info*)readMem;
	for (int i = 0; i < imageCount; ++i){
		dataCnt = 1024;
		vm_read(task, (mach_vm_address_t)dii[i].imageFilePath, dataCnt, &readMem, &dataCnt);
		char* imageName = (char*)readMem;
		if (imageName)
			g_dii[i].imageFilePath = strdup(imageName);
		else
			g_dii[i].imageFilePath = NULL;
		g_dii[i].imageLoadAddress = dii[i].imageLoadAddress;
		if (strstr(imageName, module) != NULL){
			struct stat st;
			stat(imageName, &st);
			*start = (mach_vm_address_t)dii[i].imageLoadAddress;
			*length = st.st_size;
		}
	}
	delete[] g_dii;
	return;
}
