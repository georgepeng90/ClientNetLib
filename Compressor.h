#pragma once

//����ת����
//ѹ���ͼ��ܵĹ�ͬ����
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