//
//  scanner.cpp
//
//  Created by Andre Kalisch on 23.09.16.
//  Copyright © 2016 Andre Kalisch. All rights reserved.
//

#include <sstream>

class Scanner
{
public:

    Byte* pRemote;
    const char * module;
    size_t bufferSize;
    uint64_t moduleStart;
    
    Scanner(uint64_t dwStart, size_t dwSize)
    {
        this->readBuffer(dwStart, dwSize);
    }
    
    void readBuffer(uint64_t dwStart, size_t dwSize)
    {
        pRemote = new Byte[dwSize];
        bufferSize = dwSize;
        moduleStart = dwStart;
        
        pRemote = mem->readData(moduleStart, bufferSize);
    }
    
    void unload()
    {
        delete[] pRemote;
    }
    
    bool MaskCheck(int nOffset, Byte* btPattern, const char * strMask, int sigLength)
    {
        for (int x = 0; x < sigLength; x++)
        {
            if (strMask[x] == '?')
            {
                continue;
            }

            if ((strMask[x] == 'x') && (btPattern[x] != pRemote[nOffset + x]))
            {
                return false;
            }
        }

        return true;
    }
    
    uint64_t Scan(Byte* pSignature, const char * pMask, int sigLength)
    {
        if(!pRemote)
        {
            delete[] pRemote;
            return NULL;
        }
        
        for(int dwIndex = 0; dwIndex < bufferSize; dwIndex++)
        {
            if(MaskCheck(dwIndex, pSignature, pMask, sigLength))
            {
                return moduleStart + dwIndex;
            }
        }
        
        return NULL;
    }
    
    uint32_t getOffset(Byte* pSignature, const char * pMask, uint64_t start)
    {
        uint64_t signatureAddress = this->Scan(pSignature, pMask, (int)strlen(pMask)) + start;
        uint32_t foundOffset = mem->read<uint32_t>(signatureAddress);
        
        return foundOffset;
    }
    
    uint32_t getPointer(Byte* pSignature, const char * pMask, uint32_t start)
    {
        uint64_t signatureAddress = this->Scan(pSignature, pMask, (int)strlen(pMask)) + start;
        uint64_t fileOffset = signatureAddress - moduleStart;
        uint32_t foundOffset = mem->read<uint32_t>(signatureAddress) + (uint32_t)fileOffset;
        
        return foundOffset;
    }

};
