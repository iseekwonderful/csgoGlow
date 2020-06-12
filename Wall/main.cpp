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
//  Edited by Fricker95 on 1/1/20.

/*
 launch with sudo -s
 
 stop by typing "stop", "exit" or terminating csgo
 
 usage: ./Wall [-o]
     -o: get new offset;
 
 */

#include "Scanner.hpp"
#include "Offsets.hpp"

#include <iostream>
#include <vector>
#include <thread>

bool STOP = false;

struct Color{
	float red;
	float green;
	float blue;
	float alpha;
};

void getGlowIndexOffset(mach_vm_address_t startAddress){
	/*
	 ** SHOULD NOT BE RUN ON VAC SERVERS **
	 
	 attempts to get the new glow index offset.
	 
	 - launch csgo with flag: -insecure
	 - launch game with bots.
	 - in terminal:
	  - sudo -s
	  - ./Wall -o
	 */
	
	uint64_t memoryAddress;
	
	bool isValid = false;
	std::vector<int> glowIndexes;
	
	for (int j = 0; j < m_dwGlowStructSize; j++) {
		for (int i = 0; i < 60; i++){
			memoryAddress = mem->read<uint64_t>(m_dwEntityList + (m_dwEntityStructSize * i));
			
			if (memoryAddress <= 0x0 || memoryAddress == 0)
				continue;
			
			if (!mem->read<bool>(memoryAddress + m_bDormant) && !mem->read<bool>(memoryAddress + m_bLifeState))
				glowIndexes.emplace_back(mem->read<int>(memoryAddress + m_iGlowIndex + j));
		}
		
		for (int i = 1; i < glowIndexes.size(); ++i) {
			if (glowIndexes[i] > 1 && glowIndexes[i] < 60)
				isValid = true;
			else
				isValid = false;
		}
		glowIndexes.clear();
		
		if (isValid) {
			printf("New m_iGlowIndex Offset = 0x%llx\n", m_iGlowIndex + j);
			return;
		}
		usleep(1000);
	}
	
	if (!isValid) {
		printf("Could not find new m_iGLowIndex Offset\n");
		printf("Retry using the last known Offset");
		return;
	}
}

void applyEntityGlow(mach_vm_address_t startAddress, int iTeamNum){
	Color color;
	uint64_t memoryAddress;
	int health;
	uint64_t glowBase;
	double maxFlash = 100.0f;
	
	for (int i = 0; i < 60; i++){
		memoryAddress = mem->read<uint64_t>(m_dwEntityList + (m_dwEntityStructSize * i));
		
		if (memoryAddress <= 0x0 || memoryAddress == 0)
			continue;
		
		if (!mem->read<bool>(memoryAddress + m_bDormant) && !mem->read<bool>(memoryAddress + m_bLifeState)) {
			
			health = mem->read<int>(memoryAddress + m_iHealth);
			
			if (health == 0)
				health = 100;
			
			if (mem->read<int>(memoryAddress + m_iTeam) != iTeamNum) {
				// Enemy glow colors
				color = {
					float((100 - health)/100.0),
					float((health)/100.0),
					0.0f,
					0.6f
				};
			} else {
				// Teammates glow colors
				color = {
					float((100 - health)/100.0),
					0.0f,
					float((health)/100.0),
					0.5f
				};
			}
			
			glowBase = startAddress + (m_dwGlowStructSize * mem->read<int>(memoryAddress + m_iGlowIndex));
			
			// Enables Glow
			if (glowBase != 0x0) {
				mem->write<bool>(glowBase + m_dwGlowEnable, true);
				mem->write<Color>(glowBase + m_dwGlowColorStruct, color);
			}
			
			// Anti flash
			if (m_dwLocalPlayerAddress == memoryAddress) {
				if (mem->read<double>(memoryAddress + m_dFlashAlpha) > maxFlash) {
					mem->write(memoryAddress + m_dFlashAlpha, maxFlash);
				}
				else {
					mem->write(memoryAddress + m_dFlashAlpha, 0.0f);
				}
			}
		}
	}
}

void getPointers(mach_vm_address_t client_moduleStartAddress, off_t client_moduleLength = 0) {
	Scanner* clientScanner = new Scanner(client_moduleStartAddress, client_moduleLength);
	
	m_dwLocalPlayer = client_moduleStartAddress + clientScanner->getPointer(
																			(Byte*)"\x89\xD6\x41\x89\x00\x49\x89\x00\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x00",
																			"xxxx?xx?xxx????xxxx?",
																			0xB
																			) + 0x4;
	
	m_dwEntityList = client_moduleStartAddress + clientScanner->getPointer(
																		   (Byte*)"\x48\x8D\x1D\x00\x00\x00\x00\x48\x89\xDF\xE8\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x89\xDE",
																		   "xxx????xxxx????xxx????xxx????xxx",
																		   0x3
																		   ) + 0x2C;
	
	m_dwGlowManager = client_moduleStartAddress + clientScanner->getPointer(
																			(Byte*)"\x48\x8D\x3D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x48\x8D\x35\x00\x00\x00\x00",
																			"xxx????x????xxxx????xxx????xxxxxxx????",
																			0x22
																			) + 0x4;
	
	delete clientScanner;
}

void stopThread(){
	std::string str;
	while (!STOP) {
		std::cin >> str;
		if (str == "stop" || str == "exit") {
			STOP = true;
		}
	}
}

int main(int argc, const char* argv[]) {
	
	bool getOffset = false;
	
	if (argc > 1) {
		if (std::string(argv[1]) == "-o")
			getOffset = true;
	}
	
	Process* g_cProc = new Process();
	
	mainPid = g_cProc->get("csgo_osx64");
	mainTask = g_cProc->task(mainPid);
	
	if (mainPid == -1){
		printf("Can't find CSGO's PID\n");
		if (mem)
			delete mem;
		if (g_cProc)
			delete g_cProc;
		exit(0);
	} else
		printf("Found CSGO's PID: %i\n", mainPid);
	
	off_t client_moduleLength = 0;
	mach_vm_address_t client_moduleStartAddress;
	
	if (mainTask){
		g_cProc->getModule(mainTask, &client_moduleStartAddress, &client_moduleLength, "/client.dylib");
		if (client_moduleStartAddress != 0x0) {
			printf("Found the Client.dylib address: 0x%llx\n", client_moduleStartAddress);
			printf("Module should end at 0x%llx\n", client_moduleStartAddress + client_moduleLength);
		} else{
			printf("Failed to get the Client.dylib address\n");
			if (mem)
				delete mem;
			if (g_cProc)
				delete g_cProc;
			
			exit(0);
		}
		
	} else{
		printf("Failed to get the Client.dylib address\n");
		if (mem)
			delete mem;
		if (g_cProc)
			delete g_cProc;
		
		exit(0);
	}
	
	getPointers(client_moduleStartAddress, client_moduleLength);
	
	printf("Local Player Base: 0x%llx\n", m_dwLocalPlayer);
	printf("Entity List: 0x%llx\n", m_dwEntityList);
	printf("Glow Manager: 0x%llx\n", m_dwGlowManager);
	
	uint64_t dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
	
	if (dwGlowObjectLoopStartAddress != 0x0)
		printf("Glow Object Start: 0x%llx\n", dwGlowObjectLoopStartAddress);
	
	int i_teamNum = 0;
	
	std::thread td(stopThread);
	td.detach();
	
	while (mainPid != -1 && mainTask != 0 && !STOP){
		m_dwLocalPlayerAddress = mem->read<uint64_t>(m_dwLocalPlayer);
		if (m_dwLocalPlayerAddress != 0x0) {
			i_teamNum = mem->read<int>(m_dwLocalPlayerAddress + m_iTeam);
			dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
			if (dwGlowObjectLoopStartAddress != 0x0) {
				if (getOffset) {
					getGlowIndexOffset(dwGlowObjectLoopStartAddress);
					STOP = true;
				} else {
					applyEntityGlow(dwGlowObjectLoopStartAddress, i_teamNum);
				}
			} else {
				getPointers(client_moduleStartAddress, client_moduleLength);
				dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
				if (dwGlowObjectLoopStartAddress != 0x0)
					printf("Glow Object Start: 0x%llx\n", dwGlowObjectLoopStartAddress);
			}
		}
		
		mainPid = g_cProc->get("csgo_osx64");
		usleep(800); // 800
	}
	
	STOP = true;
	
	if (mem)
		delete mem;
	if (g_cProc)
		delete g_cProc;
	
	return 0;
}
