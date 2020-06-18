//
//  Wall.hpp
//  Wall
//
//  for csgo wall hack
//  special edition
//
//  Created by sheep on 16/5/14.
//  Copyright © 2016年 nadp. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.

#pragma once
#ifndef Wall_hpp
#define Wall_hpp

#include "Scanner.hpp"
#include "Offsets.hpp"
#include "ColorText.hpp"

#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

class Wall {
	struct Color{
		float red;
		float green;
		float blue;
		float alpha;
	};
	
	off_t engine_moduleLength = 0;
	off_t client_moduleLength = 0;
	
	mach_vm_address_t engine_moduleStartAddress;
	mach_vm_address_t client_moduleStartAddress;
	
	uint64_t dwGlowObjectLoopStartAddress;
	
	Process* g_cProc = nullptr;
	MemMngr* mem = nullptr;
	
	static std::atomic<bool> stop;
	
public:
	explicit Wall() {
		stop.store(false);
		g_cProc = new Process;
		mem = new MemMngr;
		mainPid = g_cProc->get("csgo_osx64");
		mainTask = g_cProc->task(mainPid);
		
		if (mainPid == -1){
			printf("%s\n", cT::print("Error: Can't find CSGO's PID", cT::fG::red, cT::sT::bold).c_str());
			if (g_cProc)
				delete g_cProc;
			if (mem)
				delete mem;
			exit(0);
		} else {
			printf("Found CSGO's PID\t\t= %s\n", cT::print(std::to_string(mainPid).c_str(), cT::fG::green, cT::sT::bold).c_str());
		}
		
		if (mainTask){
			g_cProc->getModule(mainTask, &engine_moduleStartAddress, &engine_moduleLength, "/engine.dylib");
			
			if (engine_moduleStartAddress != 0x0) {
				printf("Found the %s address\t= %s0x%llx%s ... %s0x%llx%s\n", cT::print("engine.dylib", cT::fG::yellow).c_str(),  cT::getColor(cT::fG::green).c_str(),
					   engine_moduleStartAddress,
					   cT::getStyle(cT::sT::bold).c_str(),
					   cT::getColor(cT::fG::green).c_str(),
					   engine_moduleStartAddress +
					   engine_moduleLength,
					   cT::getStyle(cT::sT::bold).c_str());
			} else {
				printf("%s\n", cT::print("Error: Failed to get the engine.dylib address", cT::fG::red, cT::sT::bold).c_str());
				if (g_cProc)
					delete g_cProc;
				if (mem)
					delete mem;
				exit(0);
			}
			
			g_cProc->getModule(mainTask, &client_moduleStartAddress, &client_moduleLength, "/client.dylib");
			
			if (client_moduleStartAddress != 0x0) {
				printf("Found the %s address\t= %s0x%llx%s ... %s0x%llx%s\n", cT::print("client.dylib", cT::fG::yellow).c_str(),
					   cT::getColor(cT::fG::green).c_str(),
					   client_moduleStartAddress,
					   cT::getStyle(cT::sT::bold).c_str(),
					   cT::getColor(cT::fG::green).c_str(),
					   client_moduleStartAddress +
					   client_moduleLength,
					   cT::getStyle(cT::sT::bold).c_str());
			} else {
				printf("%s\n", cT::print("Error: Failed to get the client.dylib address", cT::fG::red, cT::sT::bold).c_str());
				if (g_cProc)
					delete g_cProc;
				if (mem)
					delete mem;
				exit(0);
			}
		} else {
			printf("%s\n", cT::print("Error: Failed to find main task", cT::fG::red, cT::sT::bold).c_str());
			if (g_cProc)
				delete g_cProc;
			if (mem)
				delete mem;
			exit(0);
		}
		
		getEnginePointers();
		getClientPointers();
		
		dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
		
		if (dwGlowObjectLoopStartAddress != 0x0)
			printf("Glow Object Start\t\t= %s0x%llx%s\n", cT::getColor(cT::fG::green).c_str(),  dwGlowObjectLoopStartAddress, cT::getStyle(cT::sT::bold).c_str());
	}
	
	~Wall() {
		stop.store(true);
		if (g_cProc)
			delete g_cProc;
		if (mem)
			delete mem;
	}
	
	void run(bool getOff = false) {
		std::thread td(stopThread);
		td.detach();
		
		int i_teamNum = 0;
		
		while (mainPid != -1 && mainTask != 0 && !stop.load()){
			m_dwLocalPlayerAddress = mem->read<uint64_t>(m_dwLocalPlayer);
			if (m_dwLocalPlayerAddress != 0x0) {
				i_teamNum = mem->read<int>(m_dwLocalPlayerAddress + m_iTeam);
				dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
				if (dwGlowObjectLoopStartAddress != 0x0) {
					if (getOff) {
						getOffsets();
						stop.store(true);
					} else {
						applyEntityGlow(i_teamNum);
					}
				} else {
					getClientPointers();
					dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
					if (dwGlowObjectLoopStartAddress != 0x0)
						printf("Glow Object Start\t\t= %s0x%llx%s\n", cT::getColor(cT::fG::green).c_str(),  dwGlowObjectLoopStartAddress, cT::getStyle(cT::sT::bold).c_str());
				}
			}
			
			mainPid = g_cProc->get("csgo_osx64");
			usleep(800); // 800
		}
		
		stop.store(true);
	}
	
private:
	inline static void stopThread(){
		std::string str;
		while (!stop.load()) {
			std::cin >> str;
			if (str == "stop" || str == "exit") {
				stop.store(true);
			}
		}
	}
	
	void applyEntityGlow(int iTeamNum) {
		Color color;
		uint64_t entityAddress;
		int health;
		uint64_t glowBase;
		double maxFlash = 100.0f;
		
		for (int i = 0; i < 64; ++i){
			entityAddress = mem->read<uint64_t>(m_dwEntityList + (m_dwEntityStructSize * i));
			
			if (entityAddress <= 0x0 || entityAddress == 0)
				continue;
			
			if (!mem->read<bool>(entityAddress + m_bDormant) && !mem->read<bool>(entityAddress + m_bLifeState)) {
				
				health = mem->read<int>(entityAddress + m_iHealth);
				
				if (health == 0)
					health = 100;
				
				if (mem->read<int>(entityAddress + m_iTeam) != iTeamNum) {
					// Enemy glow colors
					color.red = float((100 - health)/100.0);
					color.green = float((health)/100.0);
					color.blue = 0.0f;
					color.alpha = 0.6f;
				} else {
					// Teammates glow colors
					color.red = float((100 - health)/100.0);
					color.green = 0.0f;
					color.blue = float((health)/100.0);
					color.alpha = 0.5f;
				}
				
				glowBase = dwGlowObjectLoopStartAddress + (m_dwGlowStructSize * mem->read<int>(entityAddress + m_iGlowIndex));
				
				// Enables Glow
				if (glowBase != 0x0) {
					mem->write<bool>(glowBase + m_dwGlowEnable, true);
					mem->write<Color>(glowBase + m_dwGlowColorStruct, color);
				}
				
				// Anti flash
				if (m_dwLocalPlayerAddress == entityAddress) {
					if (mem->read<double>(entityAddress + m_dFlashAlpha) > maxFlash) {
						mem->write(entityAddress + m_dFlashAlpha, maxFlash);
					}
					else {
						mem->write(entityAddress + m_dFlashAlpha, 0.0f);
					}
				}
			}
		}
	}
	
	void getOffsets() {
		//
		// ** SHOULD NOT BE RUN ON VAC SERVERS **
		//
		// attempts to get the new glow index offset.
		//
		// - launch csgo with flag: -insecure
		// - launch game with bots.
		// - in terminal:
		// - sudo -s
		// - ./Wall -o
		//
		
		uint64_t entityAddress;
		
		bool isValid = false;
		std::vector<int> glowIndexes;
		
		for (int j = 0; j < 64; ++j) {
			for (int i = 0; i < 64; ++i){
				entityAddress = mem->read<uint64_t>(m_dwEntityList + (m_dwEntityStructSize * i));
				
				if (entityAddress <= 0x0 || entityAddress == 0)
					continue;
				
				if (!mem->read<bool>(entityAddress + m_bDormant) && !mem->read<bool>(entityAddress + m_bLifeState))
					glowIndexes.emplace_back(mem->read<int>(entityAddress + m_iGlowIndex + j));
			}
			
			for (int i = 1; i < glowIndexes.size(); ++i) {
				
				if (glowIndexes[i] > 1 && glowIndexes[i] < 60)
					isValid = true;
				else
					isValid = false;
			}
			glowIndexes.clear();
			
			if (isValid) {
				printf("\nuint64_t %s\t\t= %s0x%llx%s\n", cT::print("m_iGlowIndex", cT::fG::yellow).c_str(), cT::getColor(cT::fG::green).c_str(), m_iGlowIndex + j, cT::getStyle(cT::sT::bold).c_str());
				printf("uint64_t %s\t\t= %s0x%llx%s\n", cT::print("m_dFlashAlpha", cT::fG::yellow).c_str(), cT::getColor(cT::fG::green).c_str(), m_dFlashAlpha + j, cT::getStyle(cT::sT::bold).c_str());
				printf("uint64_t %s\t= %s0x%llx%s\n\n", cT::print("m_fFlashDuration", cT::fG::yellow).c_str(), cT::getColor(cT::fG::green).c_str(), m_fFlashDuration + j, cT::getStyle(cT::sT::bold).c_str());
				return;
			}
			usleep(1000);
		}
		
		if (!isValid) {
			printf("%s\n", cT::print("Error: Could not find new Offsets\nRetry using the last known Offsets", cT::fG::red, cT::sT::bold).c_str());
			return;
		}
	}
	
	void getEnginePointers() {
		
		Scanner* engineScanner = new Scanner(engine_moduleStartAddress, engine_moduleLength, mem);
		
		m_dwCEngineClient = engine_moduleStartAddress + engineScanner->getPointer(
																				  (Byte*)"\x55\x48\x89\xE5\x48\x8B\x00\x00\x00\x00\x00\x48\x83\x00\x00\x5D\xC3\x66\x66\x66\x66\x66\x66\x2E\x0F\x1F\x84\x00\x00\x00\x00\x00",
																				  "xxxxxx?????xx??xxxxxxxxxxxxxxxxx",
																				  0x7
																				  ) + 0x4;
		printf("Engine Client\t\t\t= %s0x%llx%s\n", cT::getColor(cT::fG::green).c_str(), m_dwCEngineClient, cT::getStyle(cT::sT::bold).c_str());
		
		delete engineScanner;
	}
	
	void getClientPointers() {
		
		Scanner* clientScanner = new Scanner(client_moduleStartAddress, client_moduleLength, mem);
		
		m_dwLocalPlayer = client_moduleStartAddress + clientScanner->getPointer(
																				(Byte*)"\x89\xD6\x41\x89\x00\x49\x89\x00\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x00",
																				"xxxx?xx?xxx????xxxx?",
																				0xB
																				) + 0x4;
		
		printf("Local Player Base\t\t= %s0x%llx%s\n", cT::getColor(cT::fG::green).c_str(), m_dwLocalPlayer, cT::getStyle(cT::sT::bold).c_str());
		
		m_dwEntityList = client_moduleStartAddress + clientScanner->getPointer(
																			   (Byte*)"\x48\x8D\x1D\x00\x00\x00\x00\x48\x89\xDF\xE8\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x89\xDE",
																			   "xxx????xxxx????xxx????xxx????xxx",
																			   0x3
																			   ) + 0x2C;
		
		printf("Entity List\t\t\t= %s0x%llx%s\n", cT::getColor(cT::fG::green).c_str(), m_dwEntityList, cT::getStyle(cT::sT::bold).c_str());
		
		m_dwGlowManager = client_moduleStartAddress + clientScanner->getPointer(
																				(Byte*)"\x48\x8D\x3D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x48\x8D\x35\x00\x00\x00\x00",
																				"xxx????x????xxxx????xxx????xxxxxxx????",
																				0x22
																				) + 0x4;
		//m_dwGlowManager = client_moduleStartAddress + 0x1E24AF0;
		printf("Glow Manager\t\t\t= %s0x%llx%s\n", cT::getColor(cT::fG::green).c_str(), m_dwGlowManager, cT::getStyle(cT::sT::bold).c_str());
		/*
		 m_dwRadarBase = client_moduleStartAddress + clientScanner->getPointer(
		 (Byte*)"\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x53\x50\x49\x89\xFD\x49\x8B\x7D\x00\x48\x85\xFF\x74\x56\x41\x8B\x75\x00\xE8\x6D\x00\x00\x00\x41\x88\xC6\x45\x84\xF6\x75\x45\x41\x80\xF6\x00\x45\x8B\x65\x00\x45\x85\xE4\x7E\x3B\x31\xDB\x4C\x8D\x3D\x00\x00\x00\x00\x66\x66\x66\x66\x66\x66\x2E\x0F\x1F\x84\x00\x00\x00\x00\x00",
		 "xxxxxxxxxxxxxxxxxxxx?xxxxxxxx?xxxxxxxxxxxxxxxx?xxx?xxxxxxxxxx????xxxxxxxxxxxxxxx",
		 0x3D
		 ) + 0x3C;
		 
		 printf("Radar Base: 0x%llx\n", m_dwRadarBase);
		*/
		/*
		 m_dwPlayerResource = client_moduleStartAddress + clientScanner->getPointer(
		 (Byte*)"\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x53\x48\x83\xEC\x00\x48\x89\xF3\x48\x8D\x7D\xB0\xBE\x00\x00\x00\x00\xE8\xAE\x2F\xDC\xFF\x4C\x8B\x75\xD0\x48\x8B\x03\x48\x8B\x80\x00\x00\x00\x00\x48\x89\xDF\xFF\xD0\x41\x80\x4E\x00\x00\x49\x89\x46\x00\x48\x8D\x1D\x00\x00\x00\x00\x48\x8B\x3B\x48\x8B\x07\x48\x8B\x80\x00\x00\x00\x00\xFF\xD0\x4C\x8D\x25\x00\x00\x00\x00",
		 "xxxxxxxxxxxxxxxx?xxxxxxxx????xxxxxxxxxxxxxxx????xxxxxxxx??xxx?xxx????xxxxxxxxx????xxxxx????",
		 0x57
		 ) + 0x4;
		 
		 printf("Player Resource: 0x%llx\n", m_dwPlayerResource);
		*/
		delete clientScanner;
	}
};

std::atomic<bool> Wall::stop;

#endif /* Wall_hpp */
