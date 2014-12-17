#ifndef __ENGINEPRIVATE_H__
#define __ENGINEPRIVATE_H__

//#define __WINDOWS__	1
//#define __LINUX__	2
#ifdef WIN32
	#define __WINDOWS__	1
#else
	#define __LINUX__	2
#endif // WIN32

#ifdef __WINDOWS__
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <windows.h>
#include <tchar.h>
#endif

#endif