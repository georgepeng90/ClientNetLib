/********************************************************************
	created:		2013/08/06  17:16
	author:			$���κ�$
	purpose:		�������Ĳ���;
*********************************************************************/

#pragma once
#include <map>
#include <vector>
#include <string>
#include "BaseType.h"

using namespace std;

//����ö��;
enum EStreamError
{
	EStreamError_Error,
	EStreamError_Ok,
	EStreamError_BufferFull,
	EStreamError_BadArg,
};

//�������ӿ�;
class BaseStream
{
protected:
	uint8	m_StreamError;		//����;
public:
	BaseStream() : m_StreamError( EStreamError_Ok ) {}
	virtual ~BaseStream() {}

	EStreamError GetError() const	{ return (EStreamError)m_StreamError; }
	bool IsOk() const				{ return (m_StreamError == EStreamError_Ok); }

#define BASESTREAM_WRITE( valueType )\
	BaseStream& operator << ( valueType value )\
	{\
		return BytesSerialize( &value, sizeof(valueType) );\
	}\
	BaseStream& operator >> ( valueType& value )\
	{\
		return BytesDeserialize( &value, sizeof(valueType) );\
	}

	BASESTREAM_WRITE( int );
	BASESTREAM_WRITE( uint );

	BASESTREAM_WRITE( int8 );
	BASESTREAM_WRITE( uint8 );

	BASESTREAM_WRITE( int16 );
	BASESTREAM_WRITE( uint16 );

	BASESTREAM_WRITE( int64 );
	BASESTREAM_WRITE( uint64 );

	BASESTREAM_WRITE( float );
	BASESTREAM_WRITE( double );

	BaseStream& operator << ( bool value )
	{
		uint8 i = ( value ? 1 : 0 );
		return BytesSerialize( &i, sizeof(uint8) );
	}

	BaseStream& operator >> ( bool& value )
	{
		uint i = 0;
		BytesDeserialize( &i, sizeof(uint8) );
		value = ( i == 1 );
		return *this;
	}

	BaseStream& operator << ( const char* str )
	{
		if( str == NULL )
		{
			m_StreamError  = EStreamError_BadArg;
			return *this;
		}
		uint size = (uint)strlen(str);
		BytesSerialize( &size, sizeof(uint) );
		return BytesSerialize( str, size );
	}

	BaseStream& operator << ( const std::string& str )
	{
		//����64λ��32λ��һ����,���data.size() > MAX_UINT,Ӧ���Ѿ���������
		uint size = (uint)str.length();
		BytesSerialize( &size, sizeof(uint) );
		return BytesSerialize( str.data(), size );
	}

    BaseStream& operator >> (std::string& str)
	{
		uint size = 0;
		BytesDeserialize( &size, sizeof(uint) );
		str.clear();
		if( size > 0 )
		{
			str.insert( 0, size, 0 );
			return BytesDeserialize( &str[0], size );
		}
		return *this;
	}

	//BaseStream& operator << ( const Guid& guid )
	//{
	//	//����64λ��32λ��һ����,���data.size() > MAX_UINT,Ӧ���Ѿ���������
	//	uint size = (uint)sizeof(Guid);
	//	return BytesSerialize( &guid, size );
	//}

	//BaseStream& operator >> ( Guid& guid )
	//{
	//	uint size = 16;
	//	BaseStream& bs = BytesDeserialize( &guid, size );
	//	return bs;
	//}

#define BASESTREAM_READ( valueType, defaultValue )\
	valueType Read_##valueType()\
	{\
		valueType value = defaultValue;\
		(*this) >> value;\
	}

	BASESTREAM_READ( int, 0 );
	BASESTREAM_READ( uint, 0 );

	BASESTREAM_READ( int8, 0 );
	BASESTREAM_READ( uint8, 0 );

	BASESTREAM_READ( int16, 0 );
	BASESTREAM_READ( uint16, 0 );

	BASESTREAM_READ( int64, 0 );
	BASESTREAM_READ( uint64, 0 );

	BASESTREAM_READ( float, 0.0f );
	BASESTREAM_READ( double, 0.0f );

    BASESTREAM_READ( string, "");
public:
	//���л� os << data;
	virtual BaseStream& BytesSerialize( const void* data, size_t size ) = 0;
	//�����л� is >> data;
	virtual BaseStream& BytesDeserialize( void* data, size_t size ) = 0;

	//ƫ��;
	virtual size_t GetOffset() const = 0;
	//�ܴ�С;
	virtual size_t GetSize() const = 0;
	//���д�С;
	virtual size_t GetSpace() const { return GetSize() - GetOffset(); }
};

//�̶������������;
class Stream : public BaseStream
{
protected:
	uint8*	m_Buffer;			//��ʼ��ַ;
	uint8*	m_Pointer;		//��ǰ��ַ;
	size_t	m_Size;			//�ܴ�С;
public:
	//�ⲿ����;
	Stream( uint8* pBuffer, size_t size );

	//�ڲ������ڴ�;
	Stream( size_t size );

	virtual ~Stream( void ){}

	//�ͷ��ڴ�;
	void ReleaseBuffer();

	//Ĭ�Ͽ�������;
	//Stream( const Stream& str );

	//����=;
	//Stream& operator = ( const Stream& str )

	//ƫ��;
	virtual size_t GetOffset() const	{ return (size_t)( m_Pointer - m_Buffer ); }
	//�ܴ�С;
	virtual size_t GetSize() const		{ return m_Size; }

	//ָ��ӿ�;
	uint8* GetBuffer() const		{ return m_Buffer; }
	uint8* GetOffsetPointer()		{ return m_Pointer; }
	void ResetPointer()			{ m_Pointer = m_Buffer; }
	void SetBuffer( uint8* pBuffer, size_t size );

	//���������м���������;
	bool SetData( size_t pos, const void* data, size_t size )
	{
		if( pos + size > m_Size )
		{
			//assert( false );
			return false;
		}
		memcpy( &(m_Buffer[pos]), data, size );
		return true;
	}

	//ƫ����ǰ;
	bool Ignore( size_t size )
	{
		if( GetSpace() < size )
		{
			//assert( false );
			m_StreamError = EStreamError_BufferFull;
			return false;
		}
		m_Pointer += size;
		return true;
	}
public:

	//���л� os << data;
	virtual BaseStream& BytesSerialize( const void* data, size_t size )
	{
		if( GetSpace() < size )
		{
			//assert( false );
			m_StreamError = EStreamError_BufferFull;
		}
		memcpy( m_Pointer, data, size );
		m_Pointer += size;
		return *this;
	}
	//�����л� is >> data;
	virtual BaseStream& BytesDeserialize( void* data, size_t size )
	{
		if( GetSpace() < size )
		{
			//assert( false );
			m_StreamError = EStreamError_BufferFull;
			return *this;
		}
		memcpy( data, m_Pointer, size );
		m_Pointer += size;
		return *this;
	}

	//��ƫ�ƴ�д��һ��ֵ
	//template<class T>
	//void SetData( T value, uint offset )
	//{
	//	if( offset + sizeof(T) > m_Size )
	//	{
	//		assert( false );
	//		m_StreamError = EStreamError_BufferFull;
	//		return;
	//	}
	//	uint8* buffer = m_Buffer + offset;
	//	*((T*)buffer) = value;
	//}

	//���л��Լ�;
	virtual void Serialize( BaseStream& streamTo ) const
	{
		if( this == &streamTo )
		{
			//assert( false );
			return;
		}
		streamTo << (int)GetOffset(); //int �Ϳͻ��˶�Ӧ
		streamTo.BytesSerialize( GetBuffer(), GetOffset() ); //д��streamTo
	}
	//�����л��Լ�;
	virtual void Deserialize( BaseStream& streamForm )
	{
		if( this == &streamForm )
		{
			//assert( false );
			m_StreamError = EStreamError_BadArg;
			return;
		}
		int dataSize = 0;	//int �Ϳͻ��˶�Ӧ
		streamForm >> dataSize;
		streamForm.BytesDeserialize( GetBuffer(), dataSize );
	}
};

//�̶���С�Ļ�����;
template< uint bufferSize>
class BufferStream : public Stream
{
protected:
	uint8	m_MemBuffer[bufferSize];
public:
	BufferStream() : Stream( m_MemBuffer, bufferSize )
	{ memset( m_MemBuffer, 0, sizeof(m_MemBuffer) ); }
};

//���л�ģ��
template<typename T>
inline BaseStream& operator << ( BaseStream& os, const T& data )
{
	data.Serialize( os );
	return os;
}

template<typename T>
inline BaseStream& operator >> ( BaseStream& is, T& data )
{
	data.Deserialize( is );
	return is;
}

//vector���л�
template<typename T>
inline BaseStream& operator << ( BaseStream& os, const std::vector<T>& data )
{
	//����64λ��32λ��һ����,���data.size() > MAX_UINT,Ӧ���Ѿ���������
	uint size = data.size();
	os << (uint)size;
	for( uint i = 0; i < size; ++i )
		os << data[i];
	return os;
}

template<typename T>
inline BaseStream& operator >> ( BaseStream& is, std::vector<T>& data )
{
	uint size = 0;
	is >> size;
	data.resize( (size_t)size );
	for( uint i = 0; i < size; ++i )
		is >> data[i];
	return is;
}

//map���л�
template<typename K, typename V>
inline BaseStream& operator << ( BaseStream& os, const std::map<K, V>& data )
{
	//����64λ��32λ��һ����,���data.size() > MAX_UINT,Ӧ���Ѿ���������
	size_t size = data.size();
	os << (uint)size;
	typename std::map<K, V>::const_iterator c_itr;
	for( c_itr = data.begin(); c_itr != data.end(); ++c_itr )
		os << (*c_itr).first << (*c_itr).second;
	return os;
}

template<typename K, typename V>
inline BaseStream& operator >> ( BaseStream& is, std::map<K, V>& data )
{
	uint size = 0;
	is >> size;
	data.clear();
	for( uint i = 0; i < size; ++i )
	{
		K key;
		is >> key >> data[key];
	}
	return is;
}