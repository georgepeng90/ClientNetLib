/********************************************************************
	created:		2013/08/06  17:16
	author:			$易鑫海$
	purpose:		数据流的操作;
*********************************************************************/

#pragma once
#include <map>
#include <vector>
#include <string>
#include "BaseType.h"

using namespace std;

//错误枚举;
enum EStreamError
{
	EStreamError_Error,
	EStreamError_Ok,
	EStreamError_BufferFull,
	EStreamError_BadArg,
};

//数据流接口;
class BaseStream
{
protected:
	uint8	m_StreamError;		//错误;
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
		//保持64位与32位的一致性,如果data.size() > MAX_UINT,应该已经出问题了
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
	//	//保持64位与32位的一致性,如果data.size() > MAX_UINT,应该已经出问题了
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
	//序列化 os << data;
	virtual BaseStream& BytesSerialize( const void* data, size_t size ) = 0;
	//反序列化 is >> data;
	virtual BaseStream& BytesDeserialize( void* data, size_t size ) = 0;

	//偏移;
	virtual size_t GetOffset() const = 0;
	//总大小;
	virtual size_t GetSize() const = 0;
	//空闲大小;
	virtual size_t GetSpace() const { return GetSize() - GetOffset(); }
};

//固定缓存的数据流;
class Stream : public BaseStream
{
protected:
	uint8*	m_Buffer;			//起始地址;
	uint8*	m_Pointer;		//当前地址;
	size_t	m_Size;			//总大小;
public:
	//外部缓存;
	Stream( uint8* pBuffer, size_t size );

	//内部分配内存;
	Stream( size_t size );

	virtual ~Stream( void ){}

	//释放内存;
	void ReleaseBuffer();

	//默认拷贝构造;
	//Stream( const Stream& str );

	//重载=;
	//Stream& operator = ( const Stream& str )

	//偏移;
	virtual size_t GetOffset() const	{ return (size_t)( m_Pointer - m_Buffer ); }
	//总大小;
	virtual size_t GetSize() const		{ return m_Size; }

	//指针接口;
	uint8* GetBuffer() const		{ return m_Buffer; }
	uint8* GetOffsetPointer()		{ return m_Pointer; }
	void ResetPointer()			{ m_Pointer = m_Buffer; }
	void SetBuffer( uint8* pBuffer, size_t size );

	//数据流的中间设置数据;
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

	//偏移向前;
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

	//序列化 os << data;
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
	//反序列化 is >> data;
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

	//在偏移处写入一个值
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

	//序列化自己;
	virtual void Serialize( BaseStream& streamTo ) const
	{
		if( this == &streamTo )
		{
			//assert( false );
			return;
		}
		streamTo << (int)GetOffset(); //int 和客户端对应
		streamTo.BytesSerialize( GetBuffer(), GetOffset() ); //写入streamTo
	}
	//反序列化自己;
	virtual void Deserialize( BaseStream& streamForm )
	{
		if( this == &streamForm )
		{
			//assert( false );
			m_StreamError = EStreamError_BadArg;
			return;
		}
		int dataSize = 0;	//int 和客户端对应
		streamForm >> dataSize;
		streamForm.BytesDeserialize( GetBuffer(), dataSize );
	}
};

//固定大小的缓存流;
template< uint bufferSize>
class BufferStream : public Stream
{
protected:
	uint8	m_MemBuffer[bufferSize];
public:
	BufferStream() : Stream( m_MemBuffer, bufferSize )
	{ memset( m_MemBuffer, 0, sizeof(m_MemBuffer) ); }
};

//序列化模板
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

//vector序列化
template<typename T>
inline BaseStream& operator << ( BaseStream& os, const std::vector<T>& data )
{
	//保持64位与32位的一致性,如果data.size() > MAX_UINT,应该已经出问题了
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

//map序列化
template<typename K, typename V>
inline BaseStream& operator << ( BaseStream& os, const std::map<K, V>& data )
{
	//保持64位与32位的一致性,如果data.size() > MAX_UINT,应该已经出问题了
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