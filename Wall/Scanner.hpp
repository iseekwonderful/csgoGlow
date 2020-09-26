//
//  Scanner.hpp
//  Wall
//
//  Created by Andre Kalisch on 23.09.16.
//  Copyright © 2016 Andre Kalisch. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "MemMngr.hpp"

#include <sstream>

class Scanner {
	MemMngr* mem = nullptr;
	Byte* pRemote = nullptr;
	const char * module = nullptr;
	size_t bufferSize;
	uint64_t moduleStart;
public:
	explicit Scanner(uint64_t dwStart, size_t dwSize, MemMngr* mem = nullptr);
	~Scanner();
	
	void readBuffer(uint64_t dwStart, uint64_t dwSize);
	bool MaskCheck(int nOffset, Byte* btPattern, const char* strMask, int sigLength);
	uint64_t Scan(Byte* pSignature, const char* pMask, int sigLength);
	uint32_t getOffset(Byte* pSignature, const char* pMask, uint64_t start);
	uint32_t getPointer(Byte* pSignature, const char* pMask, uint32_t start);
};


#endif
