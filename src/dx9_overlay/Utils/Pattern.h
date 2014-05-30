#pragma once
#include "Windows.h"

DWORD findPattern(DWORD addr, DWORD len, BYTE *bMask, char * szMask);
bool dataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);