#include "EnginePrivate.h"

//�汾˵����Windows����ϵͳ��Ҫ����꣺__WINDOWS__
//			Linux����ϵͳ��Ҫ����꣺__LINUX__
#ifndef __BASETYPE_H__
#define __BASETYPE_H__



/////////////////////////////////////////////////////////////////////////////////
//������ϢԤ�����
/////////////////////////////////////////////////////////////////////////////////
#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif /* FD_SETSIZE */


/////////////////////////////////////////////////////////////////////////////////
//��ǰ������ϵͳͷ�ļ�����
/////////////////////////////////////////////////////////////////////////////////
#if defined(__WINDOWS__)
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4996 )
	#include <Windows.h>
	#include "crtdbg.h"
#elif defined(__LINUX__)
	#include <sys/types.h>
	#include <pthread.h>
	//#include <execinfo.h>
	#include <signal.h>
	#include <exception>
	#include <setjmp.h>

    #define MSG_NOSIGNAL SO_NOSIGPIPE
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <stdarg.h>

//using namespace std;

///////////////////////////////////////////////////////////////////////
//��׼�������Ͷ���
///////////////////////////////////////////////////////////////////////
#define VOID			void				//��׼��
typedef unsigned char	UCHAR;			//��׼�޷���CHAR
typedef char			CHAR;			//��׼CHAR
typedef unsigned int	uint;			//��׼�޷���INT
typedef int			INT;				//��׼INT
typedef unsigned short	USHORT;			//��׼�޷���short
typedef short			SHORT;			//��׼short
typedef unsigned long	ULONG;			//��׼�޷���LONG(���Ƽ�ʹ��)
typedef long			LONG;			//��׼LONG(���Ƽ�ʹ��)
typedef float			FLOAT;			//��׼float

typedef UCHAR			uchar;
typedef USHORT		ushort;
typedef uint			UINT;
typedef ULONG			ulong;
typedef ULONG			IP_t;
typedef USHORT		PacketID_t;
typedef INT			BOOL;
typedef UCHAR			BYTE;

//
typedef char					int8;
typedef unsigned char			uint8;
typedef unsigned char			uchar;
typedef short					int16;
typedef unsigned short			uint16;
typedef int					int32;
typedef unsigned int			uint32;
typedef unsigned int			uint;
typedef long long				int64;
typedef unsigned long long		uint64;




///////////////////////////////////////////////////////////////////////
//�������ݺ궨��
///////////////////////////////////////////////////////////////////////

//IP��ַ���ַ���󳤶�
#define IP_SIZE			24
#define KEY_SIZE			64
#define MAX_WORLDCOUNT			256
#define TIMELENTH			23


#define PACK_COMPART "$-$"//����ָ���
#define PACK_COMPART_SIZE strlen(PACK_COMPART)

#ifndef ENCRYPT
#define ENCRYPT(x,xlen,KEY,BeginPlace)	if( (x)!=NULL ) \
		{ \
		\
			CHAR* t_pBuffer = (x); \
			CHAR* pKey = {KEY}; \
			UINT KeyLen = (UINT)strlen(pKey); \
			for (UINT i = 0; i < (xlen); i++) \
			{ \
				*t_pBuffer ^= pKey[(i+BeginPlace)%KeyLen]; \
				t_pBuffer++; \
			} \
		}
#endif

#ifndef ENCRYPT_HEAD
#define ENCRYPT_HEAD(x,KEY)	if( (x)!=NULL ) \
		{ \
			CHAR* t_pBuffer = (x); \
			CHAR* pKey = {KEY}; \
			UINT KeyLen = (UINT)strlen(pKey); \
			for (UINT i = 0; i < PACKET_HEADER_SIZE; i++) \
			{ \
				*t_pBuffer ^= pKey[i%KeyLen]; \
				t_pBuffer++; \
			} \
		}
#endif

#define OVER_MAX_SERVER 256

//��Ч�ľ��
#define INVALID_HANDLE	-1
//��Ч��IDֵ
#define INVALID_ID		-1
//��
#ifndef TRUE
	#define TRUE 1
#endif
//��
#ifndef FALSE
	#define FALSE 0
#endif
//�ļ�·�����ַ���󳤶�
#ifndef _MAX_PATH
	#define _MAX_PATH 260
#endif

///////////////////////////////////////////////////////////////////////
//����Ԥ����궨��
///////////////////////////////////////////////////////////////////////
#if defined(NDEBUG)
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#else
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#endif


#if defined(NDEBUG)
	#define _MY_TRY try
	#define _MY_CATCH catch(...)
#else
	#define _MY_TRY try
	#define _MY_CATCH catch(...)
#endif



//����ָ��ֵɾ���ڴ�
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//����ָ��ֵɾ�����������ڴ�
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
//����ָ�����free�ӿ�
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif
//����ָ�����Release�ӿ�
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif


#endif
