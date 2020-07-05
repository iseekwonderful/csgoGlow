//
//  Offsets.hpp
//  Wall
//
//  Created by Fricker95 on 1/1/20.
//  Copyright © 2020 Fricker95. All rights reserved.
//

#pragma once
#ifndef Offsets_hpp
#define Offsets_hpp

struct sOffsets {
public:
	struct sClient {
		uint64_t m_dwGlowManager;
		uint64_t m_dwGlowObjectLoopStartBase;
		uint64_t m_dwEntityList;
		uint64_t m_dwLocalPlayer;
		uint64_t m_dwLocalPlayerBase;
		uint64_t m_dwPlayerResource;
		uint64_t m_dwRadarBase;
		
		uint64_t m_dwEntityListSize					    = 0x40;
		uint64_t m_dwEntityStructSize					= 0x20;
		uint64_t m_dwGlowStructSize						= 0x40;
		uint64_t m_dwGlowColorStruct					= 0x8;
		uint64_t m_dwGlowCount							= 0x10;
		uint64_t m_dwGlowEnable							= 0x28;
		uint64_t m_iGlowIndex							= 0xAD68; // 0xAD48 // 0xAD2C
		uint64_t m_bDormant								= 0x125;
		uint64_t m_iTeam								= 0x12C;
		uint64_t m_iHealth								= 0x138;
		uint64_t m_bLifeState							= 0x297;
		uint64_t m_bSpotted								= 0xECD;
		uint64_t m_bSpottedByMask						= 0xF10;
		uint64_t m_dFlashAlpha							= 0xAD4C; // 0xAD2C
		uint64_t m_fFlashDuration						= 0xAD50; // 0xAD30
		
		/*
		 // radar base needs new sig
		 uint64_t m_dwRadarStructBase            		= 0x108;
		 uint64_t m_dwRadarDistance              		= 0x370;
		 uint64_t m_dwRadarArray                 		= 0x1E4;
		 uint64_t m_iRadarHealth                 		= 0x168;
		 uint64_t m_szRadarName                  		= 0x16C;
		 
		 // player resource needs new sig
		 uint64_t m_iKills                              = 0x1288;
		 uint64_t m_iAssists                            = 0x138C;
		 uint64_t m_iDeaths                             = 0x1490;
		 uint64_t m_bConnected                          = 0x1594;
		 uint64_t m_bHasDefuser                         = 0x1E51;
		 uint64_t m_bHasHelmet                          = 0x1E92;
		 uint64_t m_iArmor                              = 0x1ED8;
		 uint64_t m_iCompetitiveRanking                 = 0x20E0;
		 uint64_t m_iCompetitiveWins                    = 0x21E4;
		 */
	} client;
	
	struct sEngine {
		uint64_t m_dwCEngineClient;
		uint64_t m_dwCEngineClientBase;
		//uint64_t m_szGameDirectory;
		uint64_t m_dwIsInGame                           = 0x1a8; // 0x198
		//uint64_t m_dwFullUpdate                         = 0x204;
		//uint64_t m_iLocalPlayer                         = 0x210;
		//uint64_t m_szMapFile                            = 0x218;
		uint64_t m_szMapName                            = 0x32C; // 0x31C
		//uint64_t m_dwGetMaxClients                      = 0x3A0;
		//uint64_t m_setViewAngle                         = 0x8E20;
	} engine;
	
	const char* ranks[19] = {
		"Unranked",
		"Silver I",
		"Silver II",
		"Silver III",
		"Silver IV",
		"Silver Elite",
		"Silver Elite Master",
		"Gold Nova I",
		"Gold Nova II",
		"Gold Nova III",
		"Gold Nova Master",
		"Master Guardian I",
		"Master Guardian II",
		"Master Guardian Elite",
		"Distinguished Master Guardian",
		"Legendary Eagle",
		"Legendary Eagle Master",
		"Supreme Master First Class",
		"The Global Elite"
	};
};

#endif /*Offsets_hpp*/

