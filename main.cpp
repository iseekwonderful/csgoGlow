#include "Scanner.hpp"
#include "Offsets.hpp"

#include <thread>

bool STOP = false;

struct Color{
	float red;
	float green;
	float blue;
	float alpha;
};

void applyEntityGlow(mach_vm_address_t startAddress, int iTeamNum){
	Color color;
    uint64_t memoryAddress;
    int health;
    uint64_t glowBase;
	
	for (int i = 0; i < 60; i++){
		memoryAddress = mem->read<uint64_t>(m_dwEntityList + (m_dwEntityStructSize * i));
		
		if (memoryAddress <= 0x0 || memoryAddress == 0)
			continue;
		
        //int id = mem->read<int>(memoryAddress + m_iId);
        //std::cout << id << " -> ";
        
        //std::cout << mem->read<const char*>((css + m_iAccountID) + (i * 0x4));
        //std::cout << Ranks[mem->read<int>((css + m_iCompetitiveRanking) + (i * 0x4))] << "\n";
        //std::cout << *mem->read<int*>((css + m_iCompetitiveWins) + (i * 0x4)) << "\n\n";
		
		if (!mem->read<bool>(memoryAddress + m_bDormant) && !mem->read<bool>(memoryAddress + m_bLifeState)) {
			health = mem->read<int>(memoryAddress + m_iHealth);
			
			if (health == 0)
				health = 100;
			
			if (mem->read<int>(memoryAddress + m_iTeam) != iTeamNum) {
                color = {
					float((100 - health)/100.0),
					float((health)/100.0),
					0.0f,
					0.6f
				};
			} else {
				color = {
					float((100 - health)/100.0),
					0.0f,
					float((health)/100.0),
					0.5f
				};
			}
			
			glowBase = startAddress + (m_dwGlowStructSize * mem->read<int>(memoryAddress + m_iGlowIndex));
            if (glowBase != 0x0) {
                mem->write<bool>(glowBase + m_dwGlowEnable, true);
                mem->write<Color>(glowBase + m_dwGlowColorStruct, color);
            }
			
			if (m_dwLocalPlayerAddress == memoryAddress) {
				if (mem->read<double>(memoryAddress + m_dFlashAlpha) > 100) {
					mem->write(memoryAddress + m_dFlashAlpha, 100.0f);
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
    //Scanner* engineScanner = new Scanner(engine_moduleStartAddress, engine_moduleLength);
    
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
    
    m_dwPlayerResource = client_moduleStartAddress + clientScanner->getPointer(
                                                                               (Byte*)"\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x53\x48\x83\xEC\x00\x48\x89\xF3\x48\x8D\x7D\xB0\xBE\x00\x00\x00\x00\xE8\xAE\x2F\xDC\xFF\x4C\x8B\x75\xD0\x48\x8B\x03\x48\x8B\x80\x00\x00\x00\x00\x48\x89\xDF\xFF\xD0\x41\x80\x4E\x00\x00\x49\x89\x46\x00\x48\x8D\x1D\x00\x00\x00\x00\x48\x8B\x3B\x48\x8B\x07\x48\x8B\x80\x00\x00\x00\x00\xFF\xD0\x4C\x8D\x25\x00\x00\x00\x00",
                                                                               "xxxxxxxxxxxxxxxx?xxxxxxxx????xxxxxxxxxxxxxxx????xxxxxxxx??xxx?xxx????xxxxxxxxx????xxxxx????",
                                                                               0x57
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
    }
    else
        printf("Found CSGO's PID: %i\n", mainPid);
    
    off_t client_moduleLength = 0;
    mach_vm_address_t client_moduleStartAddress;
   
    /*
     off_t engine_moduleLength = 0;
     mach_vm_address_t engine_moduleStartAddress;
     g_cProc->getModule(mainTask, &engine_moduleStartAddress, & engine_moduleLength, "/engine.dylib");
     //*/
    if (mainTask){
         g_cProc->getModule(mainTask, &client_moduleStartAddress, &client_moduleLength, "/client.dylib");
        if (client_moduleStartAddress != 0x0) {
            printf("Found the Client.dylib address: 0x%llx\n", client_moduleStartAddress);
            printf("Module should end at 0x%llx\n", client_moduleStartAddress + client_moduleLength);
        } else{
            printf("Failed to get the Client.dylib address\n");
            //printf("Failed to get the Engine.dylib address\n");
            if (mem)
                delete mem;
            if (g_cProc)
                delete g_cProc;
            
            exit(0);
        }
        
        //printf("Found the Engine.dylib address: 0x%llx\n", engine_moduleStartAddress);
        //printf("Module should end at 0x%llx\n", engine_moduleStartAddress + engine_moduleLength);
    }
    else{
        printf("Failed to get the Client.dylib address\n");
        //printf("Failed to get the Engine.dylib address\n");
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
    
    /*
    uint64_t CCSPlayerResource = mem->read<uint64_t>(client_moduleStartAddress + m_dwPlayerResource);
    
    if (CCSPlayerResource != 0x0) {
        printf("CCS Player Resource Start: 0x%llx\n", CCSPlayerResource);
    }
    */
    uint64_t dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
    
    if (dwGlowObjectLoopStartAddress != 0x0) {
        printf("Glow Object Start: 0x%llx\n", dwGlowObjectLoopStartAddress);
    }
    
    int i_teamNum = 0;
    
    std::thread td(stopThread);
    td.detach();
    
    while (mainPid != -1 && mainTask != 0 && !STOP){
        m_dwLocalPlayerAddress = mem->read<uint64_t>(m_dwLocalPlayer);
        if (m_dwLocalPlayerAddress != 0x0) {
            i_teamNum = mem->read<int>(m_dwLocalPlayerAddress + m_iTeam);
            dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
            if (dwGlowObjectLoopStartAddress != 0x0) {
                applyEntityGlow(dwGlowObjectLoopStartAddress, i_teamNum);
            } else {
                getPointers(client_moduleStartAddress, client_moduleLength);
                //CCSPlayerResource = mem->read<uint64_t>(client_moduleStartAddress + m_dwPlayerResource);
                dwGlowObjectLoopStartAddress = mem->read<uint64_t>(m_dwGlowManager);
                if (dwGlowObjectLoopStartAddress != 0x0) {
                    printf("Glow Object Start: 0x%llx\n", dwGlowObjectLoopStartAddress);
                }
            }
        }
       
        mainPid = g_cProc->get("csgo_osx64");
        usleep(10000);//800
    }
    
    STOP = true;
    
    //delete engineScanner;
    if (mem)
        delete mem;
    if (g_cProc)
        delete g_cProc;
    
    return 0;
}




/*
 return (int*)((uintptr_t)this + GET_NETVAR("DT_CSPlayerResource", "m_iCompetitiveRanking") + index * 0x4);
 
 
 uintptr_t RankRevealAllPointer = client_moduleStartAddress + clientScanner->getPointer(
 (Byte*)"\x48\x89\x85\x28\xFE\xFF\xFF\x48\xC7\x85\x30\xFE\xFF\xFF\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00",
 "xxxxxxxxxxxxxx????xxx????",
 0x15
 ) + 0x4;
 printf("Rank Reveal All: 0x%lx\n", RankRevealAllPointer);
 
 float* rank_reveal = mem->read<float*>(RankRevealAllPointer);
 
 if (rank_reveal) {
 std::cout << rank_reveal << "\n";
 }
 
 //std::cout << RankRevealAllPointer << "\n";
 */



/*
	m_dwForceAttack = client_moduleStartAddress + clientScanner->getPointer(
 (Byte*)"\x8B\x00\x00\x00\x00\x00\xA8\x00\x00\x95\xC1\x00\xB6\xC9\x09\xD1\x44\x89",
 "x?????x??xx?xxxxxx",
 0x2
	) + 0x4;
 
	m_dwForceAttack2 = client_moduleStartAddress + clientScanner->getPointer(
 (Byte*)"\x8B\x00\x00\x00\x00\x00\xA8\x00\x00\x95\xC1\x00\xB6\xC9\x09\xD1\x44\x89",
 "x?????x??xx?xxxxxx",
 0x2
	) + 0x10;
 
	m_dwForceDuck = client_moduleStartAddress + clientScanner->getPointer(
 (Byte*)"\x21\xC1\x89\x00\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x89\xD1\x83\x00\x00",
 "xxx?????x?????xxx??",
 0x4
	) + 0x4;
 
	m_dwForceJump = client_moduleStartAddress + clientScanner->getPointer(
 (Byte*)"\x44\x89\xE1\xC1\xE9\x00\x83\xE1\x00\x83\xF1\x00\x21\xC1\x89\x0D\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00\x89\xD1\x83\xC9\x00\xA8\x00\x0F\x44\xCA\x44\x89\xE2\xD1\xEA\x83\xE2\x00",
 "xxxxx?xx?xx?xxxx????xx????xxxx?x?xxxxxxxxxx?",
 0x16
	) + 0x4;
 
	m_dwForceAlt1 = m_dwForceDuck + 0xC;
	m_dwForceAlt2 = m_dwForceAlt1 + 0xC;
 
	m_dwRadarBase = client_moduleStartAddress + clientScanner->getPointer(
 (Byte*)"\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x53\x50\x49\x89\xFD\x49\x8B\x7D\x00\x48\x85\xFF\x74\x56\x41\x8B\x75\x00\xE8\x6D\x00\x00\x00\x41\x88\xC6\x45\x84\xF6\x75\x45\x41\x80\xF6\x00\x45\x8B\x65\x00\x45\x85\xE4\x7E\x3B\x31\xDB\x4C\x8D\x3D\x00\x00\x00\x00\x66\x66\x66\x66\x66\x66\x2E\x0F\x1F\x84\x00\x00\x00\x00\x00",
 "xxxxxxxxxxxxxxxxxxxx?xxxxxxxx?xxxxxxxxxxxxxxxx?xxx?xxxxxxxxxx????xxxxxxxxxxxxxxx",
 0x3D
	) + 0x3C;
 
	m_dwCEngineClient = engine_moduleStartAddress + engineScanner->getPointer(
 (Byte*)"\x55\x48\x89\xE5\x48\x8B\x00\x00\x00\x00\x00\x48\x83\x00\x00\x5D\xC3\x66\x66\x66\x66\x66\x66\x2E\x0F\x1F\x84\x00\x00\x00\x00\x00",
 "xxxxxx?????xx??xxxxxxxxxxxxxxxxx",
 0x7
	) + 0x4;
 
	m_szGameDirectory = engine_moduleStartAddress + engineScanner->getPointer(
 (Byte*)"\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x53\x48\x81\xEC\x00\x00\x00\x00\x48\x89\xF3\x49\x89\xFE\x4C\x8B\x2D\x00\x00\x00\x00\x49\x8B\x45\x00\x48\x89\x45\x00\x48\x8D\x3D\x00\x00\x00\x00",
 "xxxxxxxxxxxxxxxx????xxxxxxxxx????xxxxxxx?xxx????",
 0x2C
	) + 0x4;
	//*/




/*
 printf("Force Attack: 0x%llx\n", m_dwForceAttack);
	printf("Force Attack2: 0x%llx\n", m_dwForceAttack2);
	printf("Force Duck: 0x%llx\n", m_dwForceDuck);
	printf("Force Jump: 0x%llx\n", m_dwForceJump);
	printf("Force Alt: 0x%llx\n", m_dwForceAlt1);
	printf("Force Alt2: 0x%llx\n", m_dwForceAlt2);
	printf("Radar Base: 0x%llx\n", m_dwRadarBase);
	printf("Player Resource: 0x%llx\n", m_dwPlayerResource);
 
	printf("CEngine Client: 0x%llx\n", m_dwCEngineClient);
	printf("Game Directory: 0x%llx\n", m_szGameDirectory);
	//*/

