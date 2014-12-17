#ifndef _NET_MGR_H_
#define _NET_MGR_H_

#include "EnginePrivate.h"
#include "Socket.h"
#include "StreamPacket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

/// ���������;
class CNetManager
{
public:

	//��ʼ��;
	virtual VOID	Initial();

	//ע����Ϣ;
	virtual VOID	RegisterMessage();

    //�ͷ��Լ�����ӵ�е���Դ;
	virtual VOID	Release(VOID);

    //�߼���ѭ����;
	virtual VOID	Update(VOID);

    /// ���ӵ�������;
    bool			Connect(const char *szServerAddr, INT nServerPort);

    //�Ͽ�����;
	VOID			Close(VOID);

    ///���������ݰ�;
	VOID			SendPacket(PacketHeader *packet);

protected:
	//INT DebugLog(LPCTSTR lpFormat, ...);

    //������Ϣ����;
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

	// ��������״̬;
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

	//SOCKET������;
	BOOL				Select(VOID);
	BOOL				ProcessExcept(VOID);
	BOOL				ProcessInput(VOID);
	BOOL				ProcessOutput(VOID);
	VOID				ProcessCommands(VOID);

protected:
	///��������Ϣ;
    std::string					m_strServerAddr;
	INT						    m_nServerPort;

private:
	//�ɶ����;
	fd_set					m_ReadFD;
	//��д���;
	fd_set					m_WriteFD;
	//�쳣���;
	fd_set					m_ExceptFD;
	///SOCKET;
	Socket					m_Socket;
	///���뻺����;
	SocketInputStream			m_SocketInputStream;
	///���������;
	SocketOutputStream			m_SocketOutputStream;
};


#endif // _NET_MGR_H_
