#include <stdio.h>

#define __JHASH_MIX(uia, uib, uic) \
{ \
	uia -= uib; uia -= uic; uia ^= (uic >> 13); \
	uib -= uic; uib -= uia; uib ^= (uia << 8); \
	uic -= uia; uic -= uib; uic ^= (uib >> 13); \
  \
	uia -= uib; uia -= uic; uia ^= (uic >> 12); \
	uib -= uic; uib -= uia; uib ^= (uia << 16); \
	uic -= uia; uic -= uib; uic ^= (uib >> 5); \
  \
	uia -= uib; uia -= uic; uia ^= (uic >> 3); \
	uib -= uic; uib -= uia; uib ^= (uia << 10); \
	uic -= uia; uic -= uib; uic ^= (uib >> 15); \
}

#define JHASH_GOLDEN_RATIO 0x9e3779b9

static inline unsigned int JHASH_GeneralBuffer(const void *pKey, unsigned int uiLength)
{
	unsigned int uia, uib, uic;
	unsigned int uiRemainLen;
	const unsigned char *pucKey = (const unsigned char *)pKey;
	
	uiRemainLen = uiLength;
	uia = uib = JHASH_GOLDEN_RATIO;
	uic = 0;

	while (uiRemainLen >= 12)
	{
		uia += (pucKey[0] + 
				((unsigned int)pucKey[1] << 8) +
				((unsigned int)pucKey[2] << 16) +
				((unsigned int)pucKey[3] << 24));
		uib += (pucKey[4] + 
				((unsigned int)pucKey[5] << 8) +
				((unsigned int)pucKey[6] << 16) +
				((unsigned int)pucKey[7] << 24));
		uic += (pucKey[8] + 
				((unsigned int)pucKey[9] << 8) +
				((unsigned int)pucKey[10] << 16) +
				((unsigned int)pucKey[11] << 24));
		
		__JHASH_MIX(uia, uib, uic);

		pucKey += 12;
		uiRemainLen -= 12;
	}

	switch (uiRemainLen)
	{
		case 11:
			uic += ((unsigned int)pucKey[10] << 24);
		case 10:
			uic += ((unsigned int)pucKey[9] << 16);
		case 9:
			uic += ((unsigned int)pucKey[8] << 8);
		case 8:
			uib += ((unsigned int)pucKey[7] << 24);
		case 7:
			uib += ((unsigned int)pucKey[6] << 16);
		case 6:
			uib += ((unsigned int)pucKey[5] << 8);
		case 5:
			uib += pucKey[4];
		case 4:
			uia += ((unsigned int)pucKey[3] << 24);
		case 3:
			uia += ((unsigned int)pucKey[2] << 16);
		case 2:
			uia += ((unsigned int)pucKey[1] << 8);
		case 1:
			uia += pucKey[0];
		default:
			uic += uiLength;
	}

	__JHASH_MIX(uia, uib, uic);

	return uic;
}

static inline unsigned int JHASH_U32Buffer(const unsigned int *puiKey, unsigned int uiLength)
{
	unsigned int uia, uib, uic;
	unsigned int uiRemainLen;

	uia = uib = JHASH_GOLDEN_RATIO;
	uic = 0;
	uiRemainLen = uiLength;

	while (uiRemainLen >= 3)
	{
		uia += puiKey[0];
		uib += puiKey[1];
		uic += puiKey[2];

		__JHASH_MIX(uia, uib, uic);

		puiKey += 3;
		uiRemainLen -= 3;
	}

	switch (uiRemainLen)
	{
		case 2:
			uib += puiKey[1];
		case 1:
			uia += puiKey[0];
		default:
			uic += uiLength * 4;
	}

	__JHASH_MIX(uia, uib, uic);

	return uic;
}

static inline unsigned int JHASH_3Words(unsigned int uiKey1, unsigned int uiKey2, unsigned int uiKey3)
{
	uiKey1 += JHASH_GOLDEN_RATIO;
	uiKey2 += JHASH_GOLDEN_RATIO;

	__JHASH_MIX(uiKey1, uiKey2, uiKey3);

	return uiKey3;
}

int main()
{
	unsigned int uiHashIndex = 0;
	unsigned int uiLoop = 0;
	char szMac[6] = {0x08, 0xfe, 0x06, 0x0a, 0x0a, 0xc7};

	uiHashIndex = JHASH_GeneralBuffer(szMac, 6) % 7;
	printf("Index:%u\r\n", uiHashIndex);

	for (uiLoop = 0; uiLoop < 20; uiLoop ++)
	{
		szMac[5] ++;
		uiHashIndex = JHASH_GeneralBuffer(szMac, 6) % 8;
		printf("Index:%u\r\n", uiHashIndex);
	}

	return 0;
}
