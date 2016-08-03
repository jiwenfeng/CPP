#ifndef __BASE_TIME_H__
#define __BASE_TIME_H__

#include <stdio.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#ifndef _WIN32
#include <sys/time.h>
#endif // _WIN32

#include "define.h"

class time_span
{
public:
    time_span();
    time_span(time_t time);
    time_span(long ldays, int nhours, int nmins, int nsecs);

    time_span(const time_span& timeSpanSrc);
    const time_span& operator=(const time_span& timeSpanSrc);

    int get_days() const;
    int get_hours() const;
    int get_minutes() const;
    int get_seconds() const;

    int get_total_hours() const;
    int get_total_minutes() const;
    int get_total_seconds() const;

    time_span operator-(time_span ts) const;
    time_span operator+(time_span ts) const;
    const time_span& operator+=(time_span ts);
    const time_span& operator-=(time_span ts);
    bool operator==(time_span ts) const;
    bool operator!=(time_span ts) const;
    bool operator<(time_span ts) const;
    bool operator>(time_span ts) const;
    bool operator<=(time_span ts) const;
    bool operator>=(time_span ts) const;

private:
    time_t mts;
    friend class base_time;
};	

#define CDF_DEFAULT_DATETIME_FORMAT "YYYY-MM-DD hh:mm:ss.nnn"
#define DAY_SECONDS                 (3600 * 24)
#define WEEK_SECONDS                (7 * DAY_SECONDS)

class base_time
{
public:
    base_time();
    base_time(time_t time);
    base_time(struct tm t);
    base_time(int nyear, int nmonth, int nday, int nhour, int nmin, int nsec, int ndst = -1);
    base_time(const char* str, uint32 length);
    base_time(const string& data_time);
    base_time(const base_time& timeSrc);

    const base_time& operator=(const base_time& timeSrc);
    const base_time& operator=(time_t t);

    char* format(char* psz_buffer, int imax_len, const char* szformat = CDF_DEFAULT_DATETIME_FORMAT) const;

    uint32 get_time() {return (uint32)m_time;}
    int    get_wday() {return m_wday;}

private:
    time_t m_time;
    int    m_wday;
};

extern uint32 g_run_time;

class time_utils
{
public:
    static struct timeval& get_now(struct timeval &tv);

    /**
    * ��������ʱ��Ĳ�
    * @return ���ز�ֵ, ��λ΢��
    */
    static int diff(const struct timeval& tv1, const struct timeval& tv2);

    // ��ȡ10msΪ��λȡ���Ľ������������ڵ�ʱ��
    static uint32 tenms();

    static bool is_same_hour(uint32 src, uint32 dst);

    static bool is_same_day(uint32 src, uint32 dst);

    static bool is_same_week(uint32 src, uint32 dst);

    static bool is_same_month(uint32 src, uint32 dst);

    // �����ǰʱ���Ƿ�Ϊ���ڼ������ٵ㡢���ٷ֡��������interval�������ܵķ�Χ��
    // day ���Ϊ0��ʾÿ�죬1��7��ʾ����һ��������
    static bool is_in_time_range(uint32 now, uint32 day, uint32 hour, uint32 m, uint32 sec, uint32 interval);

    static uint32 make_day_end_time();
	static uint32 make_day_start_time();

    static uint32 make_time(uint32 day, uint32 hour, uint32 m, uint32 sec);
    static uint32 make_time(time_t now, const char *fmt);    //%h:%m:%s
    static uint32 make_time(time_t now, uint32 day, uint32 hour, uint32 m, uint32 sec);

	// YYYY-MM-DD HH:mm:ss
	static int str2time(const std::string &str);

    // ��start��ʼ day ����h:m:s
    static uint32 get_time(uint32 start, uint32 day, uint32 hour, uint32 m, uint32 sec);

    // ��ȡ�ӽ��쿪ʼָ��������ָ��ʱ���ʱ���
    static uint32 make_time_special(uint32 day, uint32 hour, uint32 m, uint32 sec);

    //��ȡ���뱾����ָ�����ڵ�ʱ���
    //week >= 0
    //wday [0, 6] 0������
    static uint32 make_time_special(uint32 week, uint32 wday, uint32 hour, uint32 m, uint32 sec);

    static uint32 make_time_ext(uint32 day, uint32 hour, uint32 m, uint32 sec);
    static uint32 make_time_ext(time_t now, int32 day, int32 hour, int32 m, int32 sec);
	// ��ȡָ��������ʱ���
	static uint32 make_time_year(uint32 year,uint32 month,uint32 day,uint32 hour,uint32 m,uint32 sec);
	// ��ȡ���ܵ�ָ��ʱ���
	static uint32 make_time_day(uint32 day,uint32 hour,uint32 minute,uint32 sec);
};

class elapsed_time {
public:
    elapsed_time();
    ~elapsed_time();

private:
    struct timeval _start;
    struct timeval _end;
    unsigned long _start_ms;
    unsigned long _end_ms;
};

#define EPAPSED_TIME elapsed_time __elapsed_time;

#endif //__CTLIB_TIME_H__
