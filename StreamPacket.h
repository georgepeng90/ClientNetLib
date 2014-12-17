/********************************************************************
	created:		2013/10/29  20:48
	author:		$���κ�$
	purpose:		������
*********************************************************************/

#include "Stream.h"

#pragma once

//1�ֽڶ���
#pragma  pack(1)

//��Ϣͷ
struct PacketHeader 
{
	uint16		m_Length;			//��Ϣ����,����PacketHeader�ĳ���;
	uint16		m_Command;		//��Ϣ��;
	//������Ϣͷ��������???;
};

#pragma pack()

//��̬������,������һ����Ϣͷ;
template<class T>
class MemStreamPacket : public Stream
{
protected:
	T*				m_PacketHeader;

public:
	//��̬������ڴ�,��Ҫ�Լ��ͷ�;
	MemStreamPacket(uint16 command, uint bufferSize) : Stream(bufferSize)
	{
		//assert(bufferSize >= sizeof(T));
		if (bufferSize >= sizeof(T))
		{
			m_PacketHeader = (T*)m_Buffer;
			m_PacketHeader->m_Command = command;
			m_PacketHeader->m_Length = sizeof(T);
			m_Pointer += sizeof(T);
		}
		else
			m_PacketHeader = 0;
	}
	//�ⲿ�ڴ�;
	MemStreamPacket(uint16 command, uint8* buffer, uint bufferSize) : Stream(buffer, bufferSize)
	{
		//assert(bufferSize >= sizeof(T));
		if (bufferSize >= sizeof(T))
		{
			m_PacketHeader = (T*)m_Buffer;
			m_PacketHeader->m_Command = command;
			m_PacketHeader->m_Length = sizeof(T);
			m_Pointer += sizeof(T);
		}
		else
			m_PacketHeader = 0;
	}

	//��Ϣͷ;
	T* Header()
	{
		return m_PacketHeader;
	}

	//��ȡ��Ϣ,���Զ�����ʵ�ʳ���;
	T* Packet()
	{
		CalcLength();
		return m_PacketHeader;
	}

	//������Ϣʵ�ʳ���;
	void CalcLength()
	{
		if (m_PacketHeader == NULL)
			return;
		m_PacketHeader->m_Length = (uint16)GetOffset();
	}
};

////Ĭ�ϻ���Ļ�����;
////ע: ���̰߳�ȫ;
//class DefaultStreamPacket : public MemStreamPacket<PacketHeader>
//{
//public:
//	DefaultStreamPacket(uint16 command) : MemStreamPacket<PacketHeader>(command, s_DefaultPacketBuffer, sizeof(s_DefaultPacketBuffer))
//	{
//
//	}
//};

//�̶���С�Ļ�����, ������һ����Ϣͷ;
//����Ҫ�ͷ��ڴ�;
template<class T, uint BufferSize>
class BufferStreamPacket : public MemStreamPacket<T>
{
protected:
	uint8		m_MemBuffer[BufferSize];
	T*			m_PacketHeader;

public:
	BufferStreamPacket(uint16 command) : MemStreamPacket<T>(command, m_MemBuffer, BufferSize)
	{

	}
};

//�����ݰ�;
template<class T, uint BufferSize>
class StreamPacket
{
public:
	BufferStreamPacket<T, BufferSize>	m_Stream;

	StreamPacket() : m_Stream(0)
	{

	}

	StreamPacket(uint16 command) : m_Stream(command)
	{

	}

	//return (stream GetError() = EStreamError_OK);
	virtual bool Write(Stream& stream)
	{
		return false;
	}

	T* Packet()
	{
		Write(m_Stream);
		return m_Stream.Packet();
	}
};


typedef StreamPacket<PacketHeader, 32>				StreamPacket32;
typedef StreamPacket<PacketHeader, 64>				StreamPacket64;
typedef StreamPacket<PacketHeader, 128>				StreamPacket128;
typedef StreamPacket<PacketHeader, 256>				StreamPacket256;
typedef StreamPacket<PacketHeader, 512>				StreamPacket512;
typedef StreamPacket<PacketHeader, 1024>				StreamPacket1024;
typedef StreamPacket<PacketHeader, 2048>				StreamPacket2048;
typedef StreamPacket<PacketHeader, 4096>				StreamPacket4096;

typedef BufferStreamPacket<PacketHeader, 32>			BufferStreamPacket32;
typedef BufferStreamPacket<PacketHeader, 64>			BufferStreamPacket64;
typedef BufferStreamPacket<PacketHeader, 128>			BufferStreamPacket128;
typedef BufferStreamPacket<PacketHeader, 256>			BufferStreamPacket256;
typedef BufferStreamPacket<PacketHeader, 512>			BufferStreamPacket512;
typedef BufferStreamPacket<PacketHeader, 1024>		BufferStreamPacket1024;
typedef BufferStreamPacket<PacketHeader, 2048>		BufferStreamPacket2048;
typedef BufferStreamPacket<PacketHeader, 4096>		BufferStreamPacket4096;