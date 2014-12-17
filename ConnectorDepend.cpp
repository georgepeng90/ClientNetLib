//#include "StdAfx.h"
#include "ConnectorDepend.h"
#include <string.h>

CDataTransform* ConnecttorDepend::m_pCompressor = 0;

static CLZOCompressor theLZOCompressor;
class CConnecttorDepend
{
public:
	CConnecttorDepend()
	{
		ConnecttorDepend::m_pCompressor = &theLZOCompressor;			
	}
};

static CConnecttorDepend theDependInit;