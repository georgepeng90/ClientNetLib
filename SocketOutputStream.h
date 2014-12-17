#ifndef __SOCKETOUTPUTSTREAM_H__
#define __SOCKETOUTPUTSTREAM_H__

#include "EnginePrivate.h"
#include "BaseType.h"
#include "Socket.h"



//初始化的发送缓存长度
//#define DEFAULTSOCKETOUTPUTBUFFERSIZE 1024 * 100000
#define DEFAULTSOCKETOUTPUTBUFFERSIZE 8192
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETOUTPUTSIZE 100*1024


class Packet ;
class SocketOutputStream 
{
public :
	SocketOutputStream( Socket* sock, 
						UINT BufferSize = DEFAULTSOCKETOUTPUTBUFFERSIZE,
						UINT MaxBufferSize = DISCONNECTSOCKETOUTPUTSIZE ) ;
	virtual ~SocketOutputStream( ) ;


public :
	UINT						Write( const CHAR* buf, UINT len ) ;

	UINT						Flush( ) ;

	VOID						Initsize( ) ;
	BOOL						Resize( INT size ) ;

	INT							Capacity ()const { return m_BufferLen ; }
 
    UINT						Length( )const ;
    UINT						Size( )const { return Length( ) ; }

	CHAR*						GetBuffer( )const { return m_Buffer ; }

	CHAR*						GetTail( )const { return &(m_Buffer[m_Tail]) ; }
 
    BOOL						IsEmpty ()const { return m_Head==m_Tail ; }

	VOID						CleanUp( ) ;

	UINT						GetHead(){return m_Head;}
	UINT						GetTail(){return m_Tail;}
	UINT						GetBuffLen(){return m_BufferLen;}
	CHAR*						GetBuff(){return m_Buffer;}
//protected :
	
	Socket*		m_pSocket ;
	
	CHAR*		m_Buffer ;
	
	UINT		m_BufferLen ;
	UINT		m_MaxBufferLen ;
	
	UINT		m_Head ;
	UINT		m_Tail ;
};



#endif
