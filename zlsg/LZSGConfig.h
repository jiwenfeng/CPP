#ifndef __LZSGCONFIG_H__
#define __LZSGCONFIG_H__

#define LZSG_VERSION_MAJOR 1
#define LZSG_VERSION_MINOR 0

#ifdef _WIN32
#pragma once
#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>

#include "msvc_compat.h"

#define strcasecmp  stricmp
#define strncasecmp strnicmp 
#define sleep(X) Sleep(X)
#define snprintf sprintf_s

#define socklen_t int

/*#define __sync_fetch_and_add(V, P) InterlockedIncrement((unsigned*)V)
#define __sync_fetch_and_sub(V, P) InterlockedDecrement((unsigned*)V)
#define __sync_add_and_fetch(V, P) InterlockedExchangeAdd((unsigned*)V, P)
#define __sync_sub_and_fetch(V, P) InterlockedExchangeAdd((unsigned*)V, -P)*/

#ifdef __cplusplus
extern "C" {
#endif

static void usleep(unsigned int delay) 
{
    LARGE_INTEGER liPerfFreq = {0};
    if (!QueryPerformanceFrequency(&liPerfFreq)) {
        return;
    }

    LARGE_INTEGER liPerfStart = {0};
    QueryPerformanceCounter(&liPerfStart);

    LARGE_INTEGER liPerfNow={ 0 };
    for(;;) {
        QueryPerformanceCounter(&liPerfNow);
        double time = (((liPerfNow.QuadPart - liPerfStart.QuadPart) * 1000000) / (double)liPerfFreq.QuadPart);
        if (time >= delay)
            break;
    }
}

static char * strsep(char **stringp, const char *delim)
{
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;

    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}

static struct tm* localtime_r(const time_t* pt, struct tm* ptm) 
{
    struct tm* p = localtime(pt);
    if (ptm != NULL) {
        memcpy(ptm, p, sizeof(struct tm));
    }

    return p;
}

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone 
{
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};

static int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;

    if (NULL != tv)
    {
        GetSystemTimeAsFileTime(&ft);

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        /*converting file time to unix epoch*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS; 
        tmpres /= 10;  /*convert into microseconds*/
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    if (NULL != tz)
    {
        if (!tzflag)
        {
            _tzset();
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}

static char *strtok_r(char *s, const char *delim, char **save_ptr) 
{  
    char *token;  

    if (s == NULL) s = *save_ptr;  

    /* Scan leading delimiters.  */  
    s += strspn(s, delim);  
    if (*s == '\0')   
        return NULL;  

    /* Find the end of the token.  */  
    token = s;  
    s = strpbrk(token, delim);  
    if (s == NULL)  
        /* This token finishes the string.  */  
            *save_ptr = strchr(token, '\0');  
    else {  
        /* Terminate the token and make *SAVE_PTR point past it.  */  
        *s = '\0';  
        *save_ptr = s + 1;  
    }  

    return token;  
} 

static LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

static int clock_gettime(int X, struct timeval *tv)
{
    LARGE_INTEGER           t;
    FILETIME            f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = (LONGLONG)microseconds;
    tv->tv_sec = (long)t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}

struct timespec : public timeval {
    timespec() : tv_nsec(tv_usec) {
    }
    long& tv_nsec;
};

static char* U2G(const char* utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

#ifdef __cplusplus
}
#endif

#define CLOCK_MONOTONIC 0
#define CLOCK_REALTIME  1

#endif // _WIN32

#endif // __LZSGCONFIG_H__
