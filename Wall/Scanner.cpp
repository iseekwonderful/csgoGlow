//
//  Scanner.cpp
//  Wall
//
//  Created by Andre Kalisch on 23.09.16.
//  Copyright © 2016 Andre Kalisch. All rights reserved.
//
//  Edited by Fricker95 on 1/1/20.

#include "Scanner.hpp"

Scanner::Scanner(uint64_t dwStart, size_t dwSize, MemMngr* mem)
{
	this->mem = mem;
	this->readBuffer(dwStart, dwSize);
}

Scanner::~Scanner() {}

void Scanner::readBuffer(uint64_t dwStart, uint64_t dwSize)
{
	bufferSize = dwSize;
	moduleStart = dwStart;
	pRemote = mem->readData(moduleStart, bufferSize);
}

bool Scanner::MaskCheck(int nOffset, Byte* btPattern, const char* strMask, int sigLength)
{
	if (pRemote != 0x0) {
		for (int i = 0; i < sigLength; i++){
			if (strMask[i] == '?')
				continue;
			if ((strMask[i] == 'x') && (btPattern[i] != pRemote[nOffset + i]))
				return false;
		}
		return true;
	}
	return false;
}

uint64_t Scanner::Scan(Byte* pSignature, const char* pMask, int sigLength)
{
	if (pRemote != 0x0) {
		for (int i = 0; i < bufferSize; i++){
			if (MaskCheck(i, pSignature, pMask, sigLength))
				return moduleStart + i;
		}
	}
	return NULL;
}

uint32_t Scanner::getOffset(Byte* pSignature, const char* pMask, uint64_t start)
{
	uint64_t signatureAddress = this->Scan(pSignature, pMask, (int)strlen(pMask)) + start;
	uint32_t foundOffset = mem->read<uint32_t>(signatureAddress);
	return foundOffset;
}

uint32_t Scanner::getPointer(Byte* pSignature, const char* pMask, uint32_t start)
{
	uint64_t signatureAddress = this->Scan(pSignature, pMask, (int)strlen(pMask)) + start;
	uint64_t fileOffset = signatureAddress - moduleStart;
	uint32_t foundOffset = mem->read<uint32_t>(signatureAddress) + (uint32_t)fileOffset;
	return foundOffset;
}
