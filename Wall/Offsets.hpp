#ifndef Offsets_hpp
#define Offsets_hpp

#pragma once

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


//client.dylib
uint64_t m_dwGlowManager;
uint64_t m_dwEntityList;
uint64_t m_dwLocalPlayer;
uint64_t m_dwLocalPlayerAddress;
uint64_t m_dwForceAttack;
uint64_t m_dwForceAttack2;
uint64_t m_dwForceDuck;
uint64_t m_dwForceJump;
uint64_t m_dwForceAlt1;
uint64_t m_dwForceAlt2;
uint64_t m_dwRadarBase;
uint64_t m_dwPlayerResource;

uint64_t m_dwEntityStructSize					=0x20;
uint64_t m_dwGlowStructSize						=0x40;
uint64_t m_dwGlowColorStruct					=0x8;
uint64_t m_dwGlowEnable							=0x28;
uint64_t m_iGlowIndex							=0xAD48;	//0xAD2C;
uint64_t m_iHealth								=0x138;
uint64_t m_iTeam								=0x12C;
uint64_t m_bLifeState							=0x297;
uint64_t m_dFlashAlpha							=0xAD2C;	//0xAD10
uint64_t m_fFlashDuration						=0xAD30;	//0xAD14
uint64_t m_bDormant								=0x125;
uint64_t m_iId									=0x94;
uint64_t m_bSpotted								=0xECD;
uint64_t m_bSpottedByMask						=0xF10;
uint64_t m_Local								=0x3700;
uint64_t m_aimPunchAngle						=0x3774;
uint64_t m_viewPunchAngle						=0x3768;
//uint64_t m_vecViewOffset[0]					=0x140;
uint64_t m_nTickBase							=0x3C50;
//uint64_t m_vecVelocity[0]						=0x14C;
uint64_t m_fFlags								=0x13C;
uint64_t m_iObserverMode						=0x3B70;
uint64_t m_hObserverTarget						=0x3B84;
//uint64_t m_hViewModel[0]						=0x3AD4;
uint64_t m_szLastPlaceName						=0x3DF0;
uint64_t m_iAccount								=0xBC88; //money
//uint64_t m_angEyeAngles[0]					=0xBCA0;
//uint64_t m_angEyeAngles[1]					=0xBCA4;
uint64_t m_flLowerBodyYawTarget					=0x4378;
uint64_t m_bDeadFlag							=0x3998;
uint64_t m_vecOrigin							=0x170;
uint64_t m_nRenderMode							=0x293;
uint64_t m_Collision							=0x378;
uint64_t m_hActiveWeapon						=0x3638;
uint64_t m_hMyWeapons							=0x6A70;
uint64_t m_hMyWearables							=0x3644;
uint64_t m_iPing								=0x11C4;
uint64_t m_iKills								=0x12C8;
uint64_t m_iAssists								=0x13CC;
uint64_t m_iDeaths								=0x14D0;
uint64_t m_bConnected							=0x1180;
//uint64_t m_iTeam								=0x15D4;
uint64_t m_iPendingTeam							=0x16D8;
uint64_t m_bAlive								=0x17DC;
//uint64_t m_iHealth							=0x1820;
uint64_t m_iPlayerC4							=0x1CF4;
uint64_t m_iPlayerVIP							=0x1CF8;
uint64_t m_bHostageAlive						=0x1D14;
uint64_t m_isHostageFollowingSomeone			=0x1D20;
uint64_t m_iHostageEntityIDs					=0x1D2C;
uint64_t m_bombsiteCenterB						=0x1D08;
uint64_t m_hostageRescueX						=0x1D5C;
uint64_t m_hostageRescueY						=0x1D6C;
uint64_t m_hostageRescueZ						=0x1D7C;
uint64_t m_iMVPs								=0x1D8C;
uint64_t m_iArmor								=0x1F14;
uint64_t m_iScore								=0x2018;
uint64_t m_iCompetitiveRanking					=0x211C;
uint64_t m_iCompetitiveWins						=0x2220;
uint64_t m_iCompTeammateColor					=0x2368;
uint64_t m_bControllingBot						=0x2674;
uint64_t m_iControlledPlayer					=0x26B8;
uint64_t m_iControlledByPlayer					=0x27BC;
uint64_t m_iBotDifficulty						=0x4940;
uint64_t m_szClan								=0x4A44;
uint64_t m_iTotalCashSpent						=0x4F58;
uint64_t m_iCashSpentThisRound					=0x5160;
uint64_t m_nEndMatchNextMapVotes				=0x67FC;
uint64_t m_bEndMatchNextMapAllVoted				=0x6900;
uint64_t m_nActiveCoinRank						=0x5264;
uint64_t m_nMusicID								=0x5368;
uint64_t m_nPersonaDataPublicLevel				=0x546C;
uint64_t m_nPersonaDataPublicCommendsLeader		=0x5570;
uint64_t m_nPersonaDataPublicCommendsTeacher	=0x5674;
uint64_t m_nPersonaDataPublicCommendsFriendly	=0x5778;
uint64_t m_bBombTicking							=0x3018;
uint64_t m_flC4Blow								=0x3028;
uint64_t m_bBombDefused							=0x3048;
uint64_t m_hBombDefuser							=0x304C;
uint64_t m_iShotsFired							=0xACA0;
uint64_t m_ArmorValue							=0xBC9C;
uint64_t m_bIsDefusing							=0x4210;
uint64_t m_bIsGrabbingHostage					=0x4211;
uint64_t m_bIsScoped							=0x4208; //works
uint64_t m_bGunGameImmunity						=0x4224;
uint64_t m_bIsPlayerGhost						=0x437D;
uint64_t m_bIsRescuing							=0x4218;
uint64_t m_bHasHelmet							=0xBC90; //works
uint64_t m_bHasDefuser                          =0xBCAC; //works
uint64_t m_iItemDefinitionIndex					=0x37B2;
uint64_t m_iItemIDHigh							=0x37C8;
uint64_t m_iAccountID							=0x37D0;
uint64_t m_iEntityQuality						=0x37B4;
uint64_t m_szCustomName							=0x3888;
uint64_t m_nFallbackPaintKit					=0x3A18;
uint64_t m_nFallbackSeed						=0x3A1C;
uint64_t m_flFallbackWear						=0x3A20;
uint64_t m_nFallbackStatTrak					=0x3A24;
uint64_t m_OriginalOwnerXuidHigh				=0x3A14;
uint64_t m_OriginalOwnerXuidLow					=0x3A10;
uint64_t m_nModelIndex							=0x290;
uint64_t m_hWeapon								=0x3070;
uint64_t m_hOwner								=0x3074;
uint64_t m_fAccuracyPenalty						=0x3BA4;
uint64_t m_bStartedArming						=0x3C7C;
uint64_t m_flNextPrimaryAttack					=0x3A98;
//uint64_t m_hOwner								=0x3A90;
uint64_t m_iClip1								=0x3AC4;
uint64_t m_bShouldGlow							=0x3058;
uint64_t m_nHitboxSet							=0xFA8;

//engine.dylib
uint64_t m_dwCEngineClient;
uint64_t m_szGameDirectory;

#endif /*Offsets_hpp*/


/*
void GetWeaponId(){
	if (IsConnected == 6){
		uint64_t weaponHandle = Read<uint64_t >(LocalPlayerBase+ActiveWeaponO);
		intweaponEntID = weaponHandle&0xFFF;
		uint64_t weaponEnt = Read<uint64_t >(Client+EntityListO+(weaponEntID-1)*0x20);
		WeaponId=Read<int>(weaponEnt+WeaponIdO);
		zoom = Read<int>(weaponEnt+ZoomLevelO);
	}
}
*/


