#include "Stream.h"

using namespace std;

Stream::Stream( uint8* pBuffer, size_t size )
	: m_Buffer(pBuffer), m_Pointer(pBuffer), m_Size(size)
{
	//assert( m_Buffer );
}

Stream::Stream( size_t size )
{
	m_Size = size;
	m_Buffer = new uint8[m_Size];
	m_Pointer = m_Buffer;
}

void Stream::ReleaseBuffer()
{
	SAFE_DELETE_ARRAY( m_Buffer );
	m_Pointer = m_Buffer;
	m_Size = 0;
}

void Stream::SetBuffer( uint8* pBuffer, size_t size )
{
	m_Size = size;
	m_Buffer = pBuffer;
	m_Pointer = m_Buffer;
}