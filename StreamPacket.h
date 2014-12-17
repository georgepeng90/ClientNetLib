/********************************************************************
	created:		2013/10/29  20:48
	author:		$易鑫海$
	purpose:		缓存流
*********************************************************************/

#include "Stream.h"

#pragma once

//1字节对齐
#pragma  pack(1)

//消息头
struct PacketHeader 
{
	uint16		m_Length;			//消息长度,包括PacketHeader的长度;
	uint16		m_Command;		//消息号;
	//其他消息头增加数据???;
};

#pragma pack()

//动态缓存流,并内置一个消息头;
template<class T>
class MemStreamPacket : public Stream
{
protected:
	T*				m_PacketHeader;

public:
	//动态分配的内存,需要自己释放;
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
	//外部内存;
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

	//消息头;
	T* Header()
	{
		return m_PacketHeader;
	}

	//获取消息,并自动计算实际长度;
	T* Packet()
	{
		CalcLength();
		return m_PacketHeader;
	}

	//计算消息实际长度;
	void CalcLength()
	{
		if (m_PacketHeader == NULL)
			return;
		m_PacketHeader->m_Length = (uint16)GetOffset();
	}
};

////默认缓存的缓存流;
////注: 非线程安全;
//class DefaultStreamPacket : public MemStreamPacket<PacketHeader>
//{
//public:
//	DefaultStreamPacket(uint16 command) : MemStreamPacket<PacketHeader>(command, s_DefaultPacketBuffer, sizeof(s_DefaultPacketBuffer))
//	{
//
//	}
//};

//固定大小的缓存流, 并内置一个消息头;
//不需要释放内存;
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

//流数据包;
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