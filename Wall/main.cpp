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

void usage(const char* exec) {
	printf("\nUsage: \n");
	printf("\tsudo -s\n");
	printf("\t%s [-o] [-h]\n\n", exec);
	printf("\t-o:\tget new offsets (only use with -insecure launch option flag in CSGO)\n");
	printf("\t-h:\tdisplay this message\n\n");
}

int main(int argc, const char* argv[]) {
	
	bool getOffsets = false;
	
	if (argc > 1) {
		if (std::string(argv[1]) == "-o")
			getOffsets = true;
		if (std::string(argv[1]) == "-h"){
			usage(argv[0]);
			return 0;
		}
	}
	
	Wall wall;
	
	wall.run(getOffsets);
	
	return 0;
}
