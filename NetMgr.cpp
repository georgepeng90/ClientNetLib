#include "NetMgr.h"
#include "MessageCenter.h"

#define MAX_CONNECT_TIME	(10*100000)		// 最长连接时间10s

#ifdef __LINUX__
#include <sys/time.h>
#endif

//extern char Error[_ESIZE] ;


CNetManager::CNetManager()
	: m_SocketInputStream(&m_Socket), 
	  m_SocketOutputStream(&m_Socket)
{
	
}

CNetManager::~CNetManager()
{
}


VOID CNetManager::Initial()
{
#ifdef __WINDOWS__
	// 初始化WinSock;
	WSADATA wsaData;
	if(0!=WSAStartup( MAKEWORD( 2, 2 ), &wsaData) || (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) )
	{
		WSACleanup();
		return;
	}
#endif
}

VOID CNetManager::Update(VOID)
{
    if (m_Socket.isValid ())
    {
        //正常处理消息包;
        WaitPacket();

        //定时发送心跳消息;
        SendHeartBeat();
    }
}

VOID CNetManager::WaitConnecting(VOID)
{
	return;
}

VOID CNetManager::WaitPacket(VOID)
{
	if(!m_Socket.isValid())
	{
		return;
	}

	//网络数据流操作;
	if(!Select() || !ProcessExcept() || !ProcessInput() || !ProcessOutput())
	{
        this->Close ();
	}

	//Packet操作;
	ProcessCommands( ) ;
}

VOID CNetManager::Release(VOID)
{
	m_Socket.close();

#ifdef __WINDOWS__
	WSACleanup();
#endif
}

bool CNetManager::Connect(const char *szServerAddr, INT nServerPort)
{
    m_Socket.create();

	//连接到服务器;
	if(m_Socket.connect( szServerAddr, nServerPort ))
	{
        m_Socket.setNonBlocking ();
        m_Socket.setLinger (0);
		return true;
    }
    else
    {
        fputs ("connect to server failed \n", stdout);
		m_Socket.close();
		return false;
    }
}

VOID CNetManager::SendPacket(PacketHeader *packet)
{
	if(m_Socket.isValid())
	{
        /// write.
        m_SocketOutputStream.Write ((const char *)(packet), packet->m_Length);
	}
}

BOOL CNetManager::Select(VOID)
{
	FD_ZERO( &m_ReadFD ) ;
	FD_ZERO( &m_WriteFD ) ;
	FD_ZERO( &m_ExceptFD ) ;

	FD_SET( m_Socket.getSOCKET(), &m_ReadFD ) ;
	FD_SET( m_Socket.getSOCKET(), &m_WriteFD ) ;
	FD_SET( m_Socket.getSOCKET(), &m_ExceptFD ) ;
	
	timeval	Timeout ;
	Timeout.tv_sec = 0 ;
	Timeout.tv_usec = 0 ;

	/// windows do NOT use the first param, but unix use it and set it (maxfd + 1)
	if(SOCKET_ERROR == SocketAPI::select_ex(m_Socket.getSOCKET() + 1 , 
										&m_ReadFD , 
										&m_WriteFD , 
										&m_ExceptFD , 
										&Timeout ) )
	{
		return FALSE;
	}
	return true;
}

BOOL CNetManager::ProcessInput(VOID)
{
	if( FD_ISSET( m_Socket.getSOCKET(), &m_ReadFD ) )
	{
		uint ret = m_SocketInputStream.Fill( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			m_Socket.close() ;
			return FALSE;
		}
	}
	return true;
}

BOOL CNetManager::ProcessOutput(VOID)
{
	if( FD_ISSET( m_Socket.getSOCKET(), &m_WriteFD ) )
	{
		uint ret = m_SocketOutputStream.Flush( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			m_Socket.close() ;
			return FALSE;
		}
	}
	return true;
}

//处理消息缓冲;
VOID CNetManager::ProcessInputStream(SocketInputStream& inputStream)
{
    while( true )
    {
        /// peek msg header.
        uint header_len = sizeof(PacketHeader);

        /// have NOT one packet header.
        if (m_SocketInputStream.Length () < header_len)
            break;

        /// read header.
        PacketHeader header;
        m_SocketInputStream.Peek ((CHAR*)(&header), header_len);

        ///// error msg.
        //if (header.dwType > MSG_MAX)
        //    break;

        /// have NOT one full packet.
		if (m_SocketInputStream.Length () < (header.m_Length - header_len))
			break;

        {
            /// handle one full packet.
			// read packet.
			uint8 *buffer = new uint8[ header.m_Length ];
			m_SocketInputStream.Read ((CHAR *)(buffer), header.m_Length );
			//PacketHeader *packet = (PacketHeader*)buffer;
			Stream stream( buffer + header_len, header.m_Length - header_len );
			MessageCenter::getInstance()->receiveMessage(header.m_Command, stream);
			delete []buffer;
        }
    }
}

VOID CNetManager::ProcessCommands(VOID)
{
	ProcessInputStream(m_SocketInputStream);
}

VOID CNetManager::ProcessMsgAckLogin(VOID* msg)
{
	//BufferStreamPacket4096 packet(EClientToLoginAccountLogin);
	////			账号名				//密码;
	//packet << string("liyuandong") << string("123456");
	//SendPacket(packet.Packet());
}

BOOL CNetManager::ProcessExcept( )
{
	if( FD_ISSET( m_Socket.getSOCKET(), &m_ExceptFD ) )
	{
		m_Socket.close() ;
		return FALSE;
	}
	return true;
}


VOID CNetManager::SendHeartBeat(VOID)
{
	static time_t s_dwLastSendTime = 0;
	const UINT HEART_BEAT_RATE = 10;		// s

    time_t dwTimeNow = time (NULL);
	if(dwTimeNow - s_dwLastSendTime >= HEART_BEAT_RATE)
	{
        BufferStreamPacket4096 packet(0x0001);
		SendPacket(packet.Packet());
		s_dwLastSendTime = dwTimeNow;
	}
}


VOID CNetManager::Close(VOID)
{
	m_Socket.close();
}

// 设置网络状态;
VOID CNetManager::SetNetStatus(NETMANAGER_STATUS netStatus)
{
}

//INT CNetManager::DebugLog(LPCTSTR lpFormat, ...)
//{
//	TCHAR  Msg[1024];
//	INT  len = wvsprintf(Msg, lpFormat, va_list(1 + &lpFormat));
//	OutputDebugString(Msg);
//	return len;
//}

VOID CNetManager::RegisterMessage()
{

}
