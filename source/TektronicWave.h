#ifndef TEKTRONIC_WAVE_COMMON_HEADER_FILE
#define TEKTRONIC_WAVE_COMMON_HEADER_FILE

#define TEKTRONIC_WAVE_VERSION 1

#ifndef _signedunsigned
#define _signedunsigned signed
#endif

typedef struct _pcmsound
{
	const _signedunsigned char * pName;
	const unsigned int nSamplingRate;
	const unsigned long nLength;
} PCMSOUND,* PPCMSOUND;

#endif
