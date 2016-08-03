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
    * 计算两个时间的差
    * @return 返回差值, 单位微妙
    */
    static int diff(const struct timeval& tv1, const struct timeval& tv2);

    // 获取10ms为单位取到的进程启动到现在的时间
    static uint32 tenms();

    static bool is_same_hour(uint32 src, uint32 dst);

    static bool is_same_day(uint32 src, uint32 dst);

    static bool is_same_week(uint32 src, uint32 dst);

    static bool is_same_month(uint32 src, uint32 dst);

    // 算出当前时间是否为星期几、多少点、多少分、多少秒的interval秒所接受的范围内
    // day 如果为0表示每天，1－7表示星期一到星期天
    static bool is_in_time_range(uint32 now, uint32 day, uint32 hour, uint32 m, uint32 sec, uint32 interval);

    static uint32 make_day_end_time();
	static uint32 make_day_start_time();

    static uint32 make_time(uint32 day, uint32 hour, uint32 m, uint32 sec);
    static uint32 make_time(time_t now, const char *fmt);    //%h:%m:%s
    static uint32 make_time(time_t now, uint32 day, uint32 hour, uint32 m, uint32 sec);

	// YYYY-MM-DD HH:mm:ss
	static int str2time(const std::string &str);

    // 从start开始 day 天后的h:m:s
    static uint32 get_time(uint32 start, uint32 day, uint32 hour, uint32 m, uint32 sec);

    // 获取从今天开始指定天数后指定时间的时间戳
    static uint32 make_time_special(uint32 day, uint32 hour, uint32 m, uint32 sec);

    //获取距离本周起指定星期的时间戳
    //week >= 0
    //wday [0, 6] 0星期天
    static uint32 make_time_special(uint32 week, uint32 wday, uint32 hour, uint32 m, uint32 sec);

    static uint32 make_time_ext(uint32 day, uint32 hour, uint32 m, uint32 sec);
    static uint32 make_time_ext(time_t now, int32 day, int32 hour, int32 m, int32 sec);
	// 获取指定年月日时间撮
	static uint32 make_time_year(uint32 year,uint32 month,uint32 day,uint32 hour,uint32 m,uint32 sec);
	// 获取本周的指定时间撮
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
