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
 Usage:
 sudo -s
 ./Wall [-f <max flash alpha>] [-r <refresh rate>] [-t] [-o] [-h]
 
 -f <flash alpha>	: Antiflash alpha max amount (default: 100.0, disable: -1, range: [0-2700])
 -r <refresh rate>	: Refresh rate in microseconds (default: 2000.0)
 -t			        : Disables teammate glow
 -o			        : Get new offsets (only use with -insecure launch option flag in CSGO)
 -h			        : Display this message
 
 Stop by typing "stop", "exit" or terminating csgo
*/

#include "Wall.hpp"

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

void usage(const char* exec) {
	printf("%s\n", cT::print("\nUsage:", cT::fG::green).c_str());
	printf("\tsudo -s\n");
	printf("\t%s [-f <max flash alpha>] [-r <refresh rate>] [-o] [-h]\n\n", cT::print(exec, cT::fG::yellow).c_str());
	printf("\t-f <flash alpha>\t: Antiflash alpha max amount (default: 100.0, disable: -1, range: [0-2700])\n");
	printf("\t-r <refresh rate>\t: Refresh rate in microseconds (default: 2000.0)\n");
	printf("\t-t\t\t\t: Disables teammate glow\n");
	printf("\t-o\t\t\t: Get new offsets (only use with -insecure launch option flag in CSGO)\n");
	printf("\t-h\t\t\t: Display this message\n\n");
}

int main(int argc, char** argv) {
	
	double refreshRate = 2000.0f;
	double maxFlash = 100.0f;
	int opt;
	bool getOffsets = false;
	bool noTeammates = false;
	
	while ((opt =  getopt(argc, argv, "f:r:toh")) != -1) {
		switch (opt) {
			case 'f':
				try {
					if (strlen(optarg))
						maxFlash = std::stod(optarg);
				} catch (const std::invalid_argument&) {
					printf("%s%s%s\n", cT::print("Error: ", cT::fG::red).c_str(), optarg, cT::print(" is not a number", cT::fG::red).c_str());
					return 0;
				} catch (const std::out_of_range&) {
					printf("%s%s%s\n", cT::print("Error: ", cT::fG::red).c_str(), optarg, cT::print(" is out of range for a double", cT::fG::red).c_str());
					return 0;
				}
				break;
			case 'r':
				try {
					if (strlen(optarg))
						refreshRate = std::stod(optarg);
				} catch (const std::invalid_argument&) {
					printf("%s%s%s\n", cT::print("Error: ", cT::fG::red).c_str(), optarg, cT::print(" is not a number", cT::fG::red).c_str());
					return 0;
				} catch (const std::out_of_range&) {
					printf("%s%s%s\n", cT::print("Error: ", cT::fG::red).c_str(), optarg, cT::print(" is out of range for a double", cT::fG::red).c_str());
					return 0;
				}
				break;
			case 't':
				noTeammates = true;
				break;
			case 'o':
				getOffsets = true;
				break;
			case 'h':
				usage(argv[0]);
				exit(0);
		}
	}
	
	Wall* wall = new Wall(refreshRate, maxFlash, noTeammates);
	
	wall->run(getOffsets);
	
	delete wall;
	
	std::system("clear");
	
	return 0;
}
