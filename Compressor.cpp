//#include "StdAfx.h"
#include "compressor.h"

CDataTransform::CDataTransform(void)
{
}

CDataTransform::~CDataTransform(void)
{
}

int lzo_compress (  void *in , unsigned  in_len, void *out);
int lzo_decompress (  void *in , unsigned  in_len, void *out);

bool CLZOCompressor::Transform( char *szInBuf,int iInLen,char *szOutBuf,int &iOutLen )
{
	iOutLen = lzo_compress( (void*)szInBuf,(unsigned)iInLen,(void*)szOutBuf );
	return true;
}

bool CLZOCompressor::Detransform( char *szInBuf,int iInLen,char *szOutBuf,int &iOutLen )
{
	iOutLen = lzo_decompress( (void*)szInBuf,(unsigned)iInLen,(void*)szOutBuf );
	if( -1 == iOutLen )
		return false;
	return true;
}