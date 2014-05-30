#include "Pattern.h"

bool dataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	if (*szMask == 'x' && *pData != *bMask)
		return false;
	return (*szMask) == NULL;
}


DWORD findPattern(DWORD addr, DWORD len, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i < len; i++)
	if (dataCompare((BYTE*) (addr + i), bMask, szMask))
		return (DWORD) (addr + i);
	return 0;
}