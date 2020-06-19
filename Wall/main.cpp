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
 Launch with sudo -s
 Usage: ./Wall [-o] [-h]
     -o: get new offsets (only use with -insecure launch option flag in CSGO)
     -h: display this message
 
 Stop by typing "stop", "exit" or terminating csgo
 */

#include "Wall.hpp"

#include <stdexcept>

void usage(const char* exec) {
	printf("\nUsage: \n");
	printf("\tsudo -s\n");
	printf("\t%s [-f <max flash alpha>] [-o] [-h]\n\n", exec);
	printf("\t-f <flash alpha>:\tAntiflash flashed alpha max amount (default: 100.0, disable: -1)\n");
	printf("\t-o\t\t:\tget new offsets (only use with -insecure launch option flag in CSGO)\n");
	printf("\t-h\t\t:\tdisplay this message\n\n");
}

int main(int argc, const char* argv[]) {
	
	bool getOffsets = false;
	double maxFlash = 100.0f;
	
	if (argc > 1) {
		if (std::string(argv[1]) == "-f") {
			if (argc > 2) {
				try {
					maxFlash = std::stod(argv[2]);
				} catch (const std::invalid_argument&) {
					printf("%s%s%s\n", cT::print("Error: ", cT::fG::red).c_str(), argv[2], cT::print(" is not a number", cT::fG::red).c_str());
					return 0;
				} catch (const std::out_of_range&) {
					printf("%s%s%s\n", cT::print("Error: ", cT::fG::red).c_str(), argv[2], cT::print(" is out of range for a double", cT::fG::red).c_str());
					return 0;
				}
			}
		}
		if (std::string(argv[1]) == "-o")
			getOffsets = true;
		if (std::string(argv[1]) == "-h"){
			usage(argv[0]);
			return 0;
		}
	}
	
	Wall wall(maxFlash);
	
	wall.run(getOffsets);
	
	return 0;
}
