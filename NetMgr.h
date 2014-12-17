#ifndef _NET_MGR_H_
#define _NET_MGR_H_

#include "EnginePrivate.h"
#include "Socket.h"
#include "StreamPacket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

/// 网络管理器;
class CNetManager
{
public:

	//初始化;
	virtual VOID	Initial();

	//注册消息;
	virtual VOID	RegisterMessage();

    //释放自己所所拥有的资源;
	virtual VOID	Release(VOID);

    //逻辑轮循函数;
	virtual VOID	Update(VOID);

    /// 连接到服务器;
    bool			Connect(const char *szServerAddr, INT nServerPort);

    //断开连接;
	VOID			Close(VOID);

    ///　发送数据包;
	VOID			SendPacket(PacketHeader *packet);

protected:
	//INT DebugLog(LPCTSTR lpFormat, ...);

    //处理消息缓冲;
	VOID			ProcessInputStream(SocketInputStream& inputStream);

	VOID			ProcessMsgAckLogin(VOID* msg);

public:

	enum NETMANAGER_STATUS
	{
		CONNECT_SUCESS  = 0,
		CONNECT_FAILED_CREATE_SOCKET_ERROR,
		CONNECT_FAILED_CONNECT_ERROR,
		CONNECT_FAILED_TIME_OUT,
	};

	// 设置网络状态;
	VOID SetNetStatus(NETMANAGER_STATUS netStatus);
	static CNetManager& GetInstance()
	{
		static CNetManager s_CNetMgr;
		return s_CNetMgr;
	}

public:
	CNetManager();
	virtual ~CNetManager();

private:
	VOID				WaitConnecting(VOID);
	VOID				WaitPacket(VOID);
	VOID				SendHeartBeat(VOID);

	//SOCKET操作集;
	BOOL				Select(VOID);
	BOOL				ProcessExcept(VOID);
	BOOL				ProcessInput(VOID);
	BOOL				ProcessOutput(VOID);
	VOID				ProcessCommands(VOID);

protected:
	///服务器信息;
    std::string					m_strServerAddr;
	INT						    m_nServerPort;

private:
	//可读句柄;
	fd_set					m_ReadFD;
	//可写句柄;
	fd_set					m_WriteFD;
	//异常句柄;
	fd_set					m_ExceptFD;
	///SOCKET;
	Socket					m_Socket;
	///输入缓冲区;
	SocketInputStream			m_SocketInputStream;
	///输出缓冲区;
	SocketOutputStream			m_SocketOutputStream;
};


#endif // _NET_MGR_H_
