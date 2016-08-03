#include "base_time.h"

time_span::time_span()
{

}

time_span::time_span(time_t time)
{
    mts = time;
}

time_span::time_span(long ldays, int nhours, int nmins, int nsecs)
{
    mts = nsecs + 60* (nmins + 60* (nhours + 24* ldays));
}

time_span::time_span(const time_span& timeSpanSrc)
{
    mts = timeSpanSrc.mts;
}

const time_span& time_span::operator=(const time_span& timeSpanSrc)
{
    mts = timeSpanSrc.mts; return *this;
}

int time_span::get_days() const
{
    return static_cast<int>(mts / (24 * 3600));
}

int time_span::get_hours() const
{
    return static_cast<int>(get_total_hours() - get_days() * 24);
}

int time_span::get_minutes() const
{
    return static_cast<int>(get_total_minutes() - get_total_hours() * 60);
}

int time_span::get_seconds() const
{
    return static_cast<int>(get_total_seconds() - get_total_minutes() * 60);
}

int time_span::get_total_hours() const
{
    return static_cast<int>(mts / 3600);
}

int time_span::get_total_minutes() const
{
    return static_cast<int>(mts / 60);
}

int time_span::get_total_seconds() const
{
    return static_cast<int>(mts);
}

time_span time_span::operator-(time_span ts) const
{
    return time_span(mts - ts.mts);
}

time_span time_span::operator+(time_span ts) const
{
    return time_span(mts + ts.mts);
}

const time_span& time_span::operator+=(time_span ts)
{
    mts += ts.mts; return *this;
}

const time_span& time_span::operator-=(time_span ts)
{
    mts -= ts.mts; return *this;
}

bool time_span::operator==(time_span ts) const
{
    return mts == ts.mts;
}

bool time_span::operator!=(time_span ts) const
{
    return mts != ts.mts;
}

bool time_span::operator<(time_span ts) const
{
    return mts < ts.mts;
}

bool time_span::operator>(time_span ts) const
{
    return mts > ts.mts;
}

bool time_span::operator<=(time_span ts) const
{
    return mts <= ts.mts;
}

bool time_span::operator>=(time_span ts) const
{
    return mts >= ts.mts;
}

base_time::base_time()
{

}

base_time::base_time(time_t time)
{
    m_time = time;
}

base_time::base_time(struct tm t)
{
    m_time = mktime(&t);
}

base_time::base_time(int nyear, int nmonth, int nday, int nhour, int nmin, int nsec, int ndst /*= -1*/)
{
    struct tm atm;
    atm.tm_sec = nsec;
    atm.tm_min = nmin;
    atm.tm_hour = nhour;
    atm.tm_mday = nday;
    atm.tm_mon = nmonth - 1;        // tm_mon is 0 based
    atm.tm_year = nyear - 1900;     // tm_year is 1900 based
    atm.tm_isdst = ndst;
    m_time = mktime(&atm);
}

base_time::base_time(const char* str, uint32 length)
{
    if (length == 19) {
        struct tm stTm;
        string data_time = str;
        m_wday       = strtol(data_time.substr(8, 2).c_str(), NULL, 10);
        stTm.tm_hour = strtol(data_time.substr(11, 2).c_str(), NULL, 10);
        stTm.tm_min  = strtol(data_time.substr(14, 2).c_str(), NULL, 10);
        stTm.tm_sec  = strtol(data_time.substr(17, 2).c_str(), NULL, 10);
        m_time = stTm.tm_hour * 3600 + stTm.tm_min * 60 + stTm.tm_sec;
    } else {
        m_time = 0;
    }
}

base_time::base_time(const string& data_time)
{
    struct tm stTm;
    if (data_time.length() == 19)	// YYYY-mm-dd HH:MM:SS
    {
        uint32 year = strtol(data_time.substr(0, 4).c_str(), NULL, 10);
        //year = year < 1900 ? 1900 : year;
        uint32 mon  = strtol(data_time.substr(5, 2).c_str(), NULL, 10);
        //mon = mon < 1 ? 1 : mon;
        if (year >= 1900 && mon >= 1) {
            stTm.tm_year = year - 1900;
            stTm.tm_mon = mon - 1;
            stTm.tm_mday = strtol(data_time.substr(8, 2).c_str(), NULL, 10);
            stTm.tm_hour = strtol(data_time.substr(11, 2).c_str(), NULL, 10);
            stTm.tm_min = strtol(data_time.substr(14, 2).c_str(), NULL, 10);
            stTm.tm_sec = strtol(data_time.substr(17, 2).c_str(), NULL, 10);
            m_time = mktime(&stTm);
            if (m_time == -1) {
                m_time = 0;
            }
        } else {
            m_time = 0;
        }
    }
    else if (data_time.length() == 14) // YYYYmmddHHMMSS
    {
        stTm.tm_year = strtol(data_time.substr(0, 4).c_str(), NULL, 10) - 1900;
        stTm.tm_mon = strtol(data_time.substr(4, 2).c_str(), NULL, 10) - 1;
        stTm.tm_mday = strtol(data_time.substr(6, 2).c_str(), NULL, 10);
        stTm.tm_hour = strtol(data_time.substr(8, 2).c_str(), NULL, 10);
        stTm.tm_min = strtol(data_time.substr(10, 2).c_str(), NULL, 10);
        stTm.tm_sec = strtol(data_time.substr(12, 2).c_str(), NULL, 10);
        m_time = mktime(&stTm);
        if (m_time == -1)
        {
            m_time = 0;
        }
    }
    else if (data_time.length() == 10)	// YYYY-mm-dd
    {
        stTm.tm_year = strtol(data_time.substr(0, 4).c_str(), NULL, 10) - 1900;
        stTm.tm_mon = strtol(data_time.substr(5, 2).c_str(), NULL, 10) - 1;
        stTm.tm_mday = strtol(data_time.substr(8, 2).c_str(), NULL, 10);
        stTm.tm_hour = 0;
        stTm.tm_min = 0;
        stTm.tm_sec = 0;
        m_time = mktime(&stTm);
        if (m_time == -1)
        {
            m_time = 0;
        }
    }
    else if (data_time.length() == 8)     // YYYYmmdd
    {
        stTm.tm_year = strtol(data_time.substr(0, 4).c_str(), NULL, 10) - 1900;
        stTm.tm_mon = strtol(data_time.substr(4, 2).c_str(), NULL, 10) - 1;
        stTm.tm_mday = strtol(data_time.substr(6, 2).c_str(), NULL, 10);
        stTm.tm_hour = 0;
        stTm.tm_min = 0;
        stTm.tm_sec = 0;
        m_time = mktime(&stTm);
        if (m_time == -1)
        {
            m_time = 0;
        }
    }
    else
    {
        m_time = 0;
    }
}

base_time::base_time(const base_time& timeSrc)
{
    m_time = timeSrc.m_time;
}

const base_time& base_time::operator=(const base_time& timeSrc)
{
    m_time = timeSrc.m_time; return *this;
}

const base_time& base_time::operator=(time_t t)
{
    m_time = t; return *this;
}

char* base_time::format(char* psz_buffer, int imax_len, const char* szformat /*= CDF_DEFAULT_DATETIME_FORMAT*/) const
{
    time_t time = m_time;
    struct tm ptm_temp;
    if (localtime_r(&time, &ptm_temp) == NULL || !strftime(psz_buffer, imax_len, szformat, &ptm_temp))
    {
        psz_buffer[0] = '\0';
    }
    return psz_buffer;
}

struct timeval& time_utils::get_now(struct timeval &tv)
{
    gettimeofday(&tv, NULL);
    return tv;
}

int time_utils::diff(const struct timeval& tv1, const struct timeval& tv2)
{
    return (tv1.tv_sec - tv2.tv_sec) * 1000000 + 
        (tv1.tv_usec - tv2.tv_usec);
}

int time_utils::str2time(const std::string &str)
{
	time_t now = time(NULL);
	struct tm t;
	localtime_r(&now, &t);
	sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
	t.tm_year -= 1900;
	t.tm_mon -= 1;
	return (int)mktime(&t);
}

uint32 time_utils:: make_time_year(uint32 year,uint32 month,uint32 day,uint32 hour,uint32 m,uint32 sec)
{
	time_t now = time(NULL);
	struct tm t;
	localtime_r(&now, &t);
	t.tm_year = year - 1900u;
	t.tm_mon = month -1;
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = m;
	t.tm_sec = sec;

	return (uint32)mktime(&t);
}

uint32 time_utils::make_time_day(uint32 day,uint32 hour,uint32 minute,uint32 sec)
{
	time_t now = time(NULL);
	struct tm t;
	localtime_r(&now, &t);
	int cur_day = static_cast<int>(day);
	
	int diff;
	if(t.tm_wday == 0 && day == 7){
		diff = 0;
	}
	else{

		diff = (cur_day - t.tm_wday) * 24 * 3600;
	}
	t.tm_hour = hour;
	t.tm_min = minute;
	t.tm_sec = sec;

	int mk_time = (int)mktime(&t) + diff;

	return (uint32)mk_time;
}

uint32 time_utils::tenms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec - g_run_time) * 100 + (tv.tv_usec / 10000));
}

bool time_utils::is_same_hour(uint32 src, uint32 dst)
{
    time_t tsrc = src;
    time_t tdst = dst;
    struct tm psrc;
    localtime_r(&tsrc, &psrc);
    struct tm pdst;
    localtime_r(&tdst, &pdst);
    return (psrc.tm_year == pdst.tm_year && psrc.tm_yday == pdst.tm_yday && psrc.tm_hour == pdst.tm_hour);
}

bool time_utils::is_same_day(uint32 src, uint32 dst)
{
    int ret = dst - src;
    if (ret < -86400 || ret > 86400) {
        return false;
    }
    time_t tsrc = src;
    time_t tdst = dst;
    struct tm psrc;
    localtime_r(&tsrc, &psrc);
    struct tm pdst;
    localtime_r(&tdst, &pdst);
    return (psrc.tm_year == pdst.tm_year && psrc.tm_yday == pdst.tm_yday);
}

bool time_utils::is_same_week(uint32 src, uint32 dst)
{
    uint32 min = src < dst ? src : dst;
    uint32 max = src > dst ? src : dst;
    time_t tsrc = min;
    time_t tdst = max;
    struct tm psrc;
    localtime_r(&tsrc, &psrc);
    struct tm pdst;
    localtime_r(&tdst, &pdst);
    int yday = pdst.tm_yday - psrc.tm_yday;
    int wday = pdst.tm_wday - psrc.tm_wday;
    return psrc.tm_year == pdst.tm_year && yday < 7 && yday == wday;
}

bool time_utils::is_same_month(uint32 src, uint32 dst)
{
    time_t tsrc = src;
    time_t tdst = dst;
    struct tm psrc;
    localtime_r(&tsrc, &psrc);
    struct tm pdst;
    localtime_r(&tdst, &pdst);
    return (psrc.tm_year == pdst.tm_year && psrc.tm_mon == pdst.tm_mon);
}

bool time_utils::is_in_time_range(uint32 now, uint32 day, uint32 hour, uint32 m, uint32 sec, uint32 interval)
{
    time_t tnow = now;
    struct tm pnow;
    if (localtime_r(&tnow, &pnow) == NULL) {
        return false;
    }

    if (day != 0) { // 星期几天数限定
        uint32 tm_wday = pnow.tm_wday;
        if (tm_wday == 0) {
            tm_wday = 7;
        }/* else {
            ++tm_wday;
        }*/

        if (day != tm_wday) {
            return false;
        }
    }

    // 确定为今天可以
    struct tm pneed;
    memcpy(&pneed, &pnow, sizeof(struct tm));
    pneed.tm_hour = hour;
    pneed.tm_min = m;
    pneed.tm_sec = sec;
    uint32 need = (uint32)mktime(&pneed);
    if (now < need) {  // 时间未到
        return false;
    }
    if (now > need + interval) { // 秒数已过
        return false;
    } 

    return true;
}

uint32 time_utils::make_day_start_time()
{
    time_t now = time(NULL);
    struct tm mt;
    if (localtime_r(&now, &mt) == NULL) {
        return 0;
    }

    mt.tm_hour = 0;
    mt.tm_min = 0;
    mt.tm_sec = 0;
    return (uint32)mktime(&mt);
}

uint32 time_utils::make_day_end_time()
{
    time_t now = time(NULL);
    struct tm mt;
    if (localtime_r(&now, &mt) == NULL) {
        return 0;
    }

    mt.tm_hour = 23;
    mt.tm_min = 59;
    mt.tm_sec = 59;
    return (uint32)mktime(&mt);
}

uint32 time_utils::make_time(uint32 day, uint32 hour, uint32 m, uint32 sec)
{
    return make_time(time(NULL), day, hour, m, sec);
}

uint32 time_utils::make_time(time_t now, const char *fmt)
{
    int h = 0, m = 0, s = 0;
    sscanf(fmt, "%d:%d:%d", &h, &m, &s);
    struct tm mt;
    if(localtime_r(&now, &mt) == NULL) {
        return false;
    }
    mt.tm_hour = h;
    mt.tm_min = m;
    mt.tm_sec = s;
    return (uint32)mktime(&mt);
}

uint32 time_utils::make_time(time_t now, uint32 day, uint32 hour, uint32 m, uint32 sec)
{
    struct tm mt;
    if (localtime_r(&now, &mt) == NULL) {
        return false;
    }

    if (day != 0) {
        int old_wday = mt.tm_wday;
        mt.tm_wday = day % 7;
        if (old_wday > mt.tm_wday) {
            mt.tm_mday += 7 - (old_wday - mt.tm_wday);
            mt.tm_yday += 7 - (old_wday - mt.tm_wday);
        } else {
            mt.tm_mday += mt.tm_wday - old_wday;
            mt.tm_yday += mt.tm_wday - old_wday;
        }
    }

    mt.tm_hour = hour;
    mt.tm_min  = m;
    mt.tm_sec  = sec;
    return (uint32)mktime(&mt);
}

uint32 time_utils::get_time(uint32 start, uint32 day, uint32 hour, uint32 m, uint32 sec)
{
    time_t s = (time_t)(start + day * 24 * 3600);
    struct tm *tm = localtime(&s);
    tm->tm_hour = hour;
    tm->tm_min = m;
    tm->tm_sec = sec;
    return (uint32)mktime(tm);
}

uint32 time_utils::make_time_special(uint32 day, uint32 hour, uint32 m, uint32 sec)
{
    time_t tt0 = time(NULL);
    struct tm *tm = localtime(&tt0);
    tm->tm_hour = hour;
    tm->tm_min = m;
    tm->tm_sec = sec;
    time_t tt1 = mktime(tm);
    int diff_secs = (int)(tt1 - tt0);
    if(diff_secs < 0 && day <= 0)
    {
        day = 1;
    }
    return (uint32)tt0 + day * 24 * 3600 + diff_secs;
}

uint32 time_utils::make_time_special(uint32 week, uint32 wday, uint32 hour, uint32 m, uint32 sec)
{
    time_t tt0 = time(NULL);
    struct tm *tm = localtime(&tt0);
    tm->tm_hour = hour;
    tm->tm_min = m;
    tm->tm_sec = sec;
    time_t tt1 = mktime(tm);
    int diff_days = wday - tm->tm_wday + 7 * week;		// 相差的天数
    diff_days = diff_days >= 0 ? diff_days : 7 + diff_days;
    int diff_secs = (int)(tt1 - tt0);
	if(diff_secs < 0 && diff_days == 0)
	{
		diff_days += 7;
	}
    return (uint32)(tt0 + diff_days * 24 * 3600 + diff_secs);
}

uint32 time_utils::make_time_ext(uint32 day, uint32 hour, uint32 m, uint32 sec)
{
    return make_time_ext(time(NULL), day, hour, m, sec);
}

uint32 time_utils::make_time_ext(time_t now, int32 day, int32 hour, int32 m, int32 sec)
{
    struct tm mt;
    if (localtime_r(&now, &mt) == NULL) {
        return false;
    }

    if (day != 0) {
        int old_wday = mt.tm_wday;
        mt.tm_wday = day % 7;
        if (old_wday > mt.tm_wday ||
            (old_wday == mt.tm_wday && mt.tm_hour > hour) ||
            (old_wday == mt.tm_wday && mt.tm_hour == hour && mt.tm_min > m) ||
            (old_wday == mt.tm_wday && mt.tm_hour == hour && mt.tm_min == m && mt.tm_sec > sec)) {
                mt.tm_mday += 7 - (old_wday - mt.tm_wday);
                mt.tm_yday += 7 - (old_wday - mt.tm_wday);
        } else {
            mt.tm_mday += mt.tm_wday - old_wday;
            mt.tm_yday += mt.tm_wday - old_wday;
        }
    } else {
        if ((mt.tm_hour > hour) ||
            (mt.tm_hour == hour && mt.tm_min > m) ||
            (mt.tm_hour == hour && mt.tm_min == m && mt.tm_sec > sec)) {
                mt.tm_wday = mt.tm_wday + 1 == 7 ? 0 : mt.tm_wday + 1;
                mt.tm_mday += 1;
                mt.tm_yday += 1;
        }
    }

    mt.tm_hour = hour;
    mt.tm_min  = m;
    mt.tm_sec  = sec;
    return (uint32)mktime(&mt);
}

elapsed_time::elapsed_time()
{
    gettimeofday(&_start, NULL);
    _start_ms = ((long)_start.tv_sec) * 1000 + (long)_start.tv_usec / 1000;
    printf("Start time: %lu ms\n", _start_ms);
}

elapsed_time::~elapsed_time()
{
    gettimeofday(&_end, NULL);
    _end_ms = ((long)_end.tv_sec) * 1000 + (long)_end.tv_usec / 1000;
    printf("End time: %lu ms\nelapsed time: %lu ms\n", _end_ms, _end_ms - _start_ms);
}
