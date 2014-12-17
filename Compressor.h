#pragma once

//数据转换器
//压缩和加密的共同基类
class CDataTransform
{
public:
	CDataTransform(void);
	~CDataTransform(void);

	virtual bool Transform( char *szInBuf,int iInLen,char *szOutBuf,int &iOutLen ) = 0;
	virtual bool Detransform( char *szInBuf,int iInLen,char *szOutBuf,int &iOutLen ) = 0;
};

class CLZOCompressor:public CDataTransform
{
public:
	virtual bool Transform( char *szInBuf,int iInLen,char *szOutBuf,int &iOutLen );
	virtual bool Detransform( char *szInBuf,int iInLen,char *szOutBuf,int &iOutLen );
};