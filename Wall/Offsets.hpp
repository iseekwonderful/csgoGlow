//
//  Offsets.hpp
//
//  Created by Fricker95 on 1/1/20.
//  Copyright © 2020 Fricker95. All rights reserved.
//

#ifndef Offsets_hpp
#define Offsets_hpp

#pragma once

//client.dylib
uint64_t m_dwGlowManager;
uint64_t m_dwEntityList;
uint64_t m_dwLocalPlayer;
uint64_t m_dwLocalPlayerAddress;

uint64_t m_dwEntityStructSize					=0x20;
uint64_t m_dwGlowStructSize						=0x40;
uint64_t m_dwGlowColorStruct					=0x8;
uint64_t m_dwGlowEnable							=0x28;
uint64_t m_iGlowIndex							=0xAD68; //0xAD48
uint64_t m_iHealth								=0x138;
uint64_t m_iTeam								=0x12C;
uint64_t m_bLifeState							=0x297;
uint64_t m_dFlashAlpha							=0xAD2C;
uint64_t m_fFlashDuration						=0xAD30;
uint64_t m_bDormant								=0x125;
uint64_t m_bSpotted								=0xECD;
uint64_t m_bSpottedByMask						=0xF10;

#endif /*Offsets_hpp*/

/*
const char* Ranks[] =
{
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
*/


