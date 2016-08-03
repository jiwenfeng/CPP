#ifndef __CFG_PARSER_H__
#define __CFG_PARSER_H__

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <map>
#include "rect.h"
#include "base_time.h"

#ifdef _WIN32
#define bzero ZeroMemory
#endif 

class cfg_parser
{
public:
    cfg_parser(void);
    virtual ~cfg_parser(void);

public:
    template <typename T> 
    bool parser(const char* cstr, const char* delimiters, T& obj);
};

template <typename T> 
bool cfg_parser::parser(const char* cstr, const char* delimiters, T& obj) {
    size_t len = strlen(cstr);
    if (len < 1) {
        return true;
    }

    const char* next = NULL;
    //char *next_token = NULL;
    char* str = new char[len + 1];
	bzero(str, len + 1);
    char *saveptr1 = NULL;
    strcpy(str, cstr);
#ifndef _WIN32
    next = strtok_r(str, delimiters, &saveptr1);
#else 
    next = strtok_s(str, delimiters, &saveptr1);
#endif // _WIN32
    bool ret = false;
    while (next != NULL) {
        ret = obj(next);
        if (!ret) {
            delete []str;
            return ret;
        }
#ifndef _WIN32
        next = strtok_r(NULL, delimiters, &saveptr1);
#else
        next = strtok_s(NULL, delimiters, &saveptr1);
#endif // _WIN32
    }

    delete []str;
    return ret;
}

// cp.parser<cpf>("4;5;6", ";", p)
class cpf {
public:
    cpf(std::vector<int>* v = NULL) 
            : _cfg(v) {
    }

    void set_cfg(std::vector<int>* v) {
        _cfg = v;
    }

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        if (_cfg == NULL) {
            cfg.push_back(atoi(str));
        } else {
            _cfg->push_back(atoi(str));
        }

        return true;
    }

    std::vector<int> cfg;
    std::vector<int>* _cfg;
};

// cp.parser<cpf_0>("7|7;8|8;9|9", ";", p0);
class cpf_0 {
public:
    cpf_0(std::map<int, int>* p = NULL) 
            : _cfg(p) {
    }

    void set_cfg(std::map<int, int>* p) {
        _cfg = p;
    }

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        cpf p;
        cfg_parser cp;
        cp.parser<cpf>(str, "|", p);
        if (p.cfg.size() < 2) {
            return false;
        }

        if (_cfg == NULL) {
            cfg.insert(std::pair<int, int>(p.cfg[0], p.cfg[1]));
        } else {
            _cfg->insert(std::pair<int, int>(p.cfg[0], p.cfg[1]));
        }
        return true;
    }

    std::map<int, int> cfg;
    std::map<int, int>* _cfg;
};


// cp.parser<cpf_1>("7_7;8_8;9_9", ";", p1);
class cpf_1 {
public:
    cpf_1(std::map<int, int>* p = NULL) 
            : _cfg(p) {
    }

    void set_cfg(std::map<int, int>* p) {
        _cfg = p;
    }

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        cpf p;
        cfg_parser cp;
        cp.parser<cpf>(str, "_", p);
        if (p.cfg.size() < 2) {
            return false;
        }

        if (_cfg == NULL) {
            cfg.insert(std::pair<int, int>(p.cfg[0], p.cfg[1]));
        } else {
            _cfg->insert(std::pair<int, int>(p.cfg[0], p.cfg[1]));
        }
        return true;
    }

    std::map<int, int> cfg;
    std::map<int, int>* _cfg;
};

// struct id_count_profession {
//     uint32 id;
//     uint32 count;
//     uint32 profession;
// };

// cp.parser<cpf_1>("7_7_7;8_8_8;9_9_9", ";", p1);
class cpf_1_0 {
public:
    cpf_1_0(std::map<int, std::map<int, int> >* p = NULL) 
        : _cfg(p) {
    }

    void set_cfg(std::map<int, std::map<int, int> >* p) {
        _cfg = p;
    }

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        cpf p;
        cfg_parser cp;
        cp.parser<cpf>(str, "_", p);
        if (p.cfg.size() == 0) {
            return false;
        }

        if (p.cfg.size() == 1) {
            p.cfg.push_back(1);
        }

        if (p.cfg.size() == 2) {
            p.cfg.push_back(5);
        }

        //id_count_profession val;
        //val.id = p.cfg[0];
        //val.count = p.cfg[1];
        //val.profession = p.cfg[2];
        int val = p.cfg[1];

        if (_cfg == NULL) {
            cfg[p.cfg[2]].insert(std::pair<int, int>(p.cfg[0], val));
        } else {
            (*_cfg)[p.cfg[2]].insert(std::pair<int, int>(p.cfg[0], val));
        }
        return true;
    }

    std::map<int, std::map<int, int> > cfg;
    std::map<int, std::map<int, int> >* _cfg;
};

struct quest_reward_item
{
    quest_reward_item() : id(0), num(0), profession(5)/*, level(1), attr(0) */{

    }

    int id;
    int num;
    int profession;
//    int level;
//    int attr;
};

// cp.parser<cpf_1>("7_7_7_7_7;8_8_8_8_8;9_9_9_9_9", ";", p1);
class cpf_quest_reward_item {
public:
    cpf_quest_reward_item(std::vector<quest_reward_item>* p) 
        : _cfg(p) {
    }

    void set_cfg(std::vector<quest_reward_item>* p) {
        _cfg = p;
    }

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        cpf p;
        cfg_parser cp;
        cp.parser<cpf>(str, "_", p);
        if (p.cfg.size() == 0) {
            return false;
        }

        quest_reward_item val;

        if (p.cfg.size() >= 1) {
            val.id = p.cfg[0];
        }

        if (p.cfg.size() >= 2) {
            val.num = p.cfg[1];
        }

        if (p.cfg.size() >= 3) {
            val.profession = p.cfg[2];
        }

//        if (p.cfg.size() >= 4) {
//            val.level = p.cfg[3];
//        }

//        if (p.cfg.size() >= 5) {
//            val.attr = p.cfg[4];
//        }

        if (_cfg == NULL) {
            cfg.push_back(val);
        } else {
            _cfg->push_back(val);
        }

        return true;
    }

    std::vector<quest_reward_item> cfg;
    std::vector<quest_reward_item>* _cfg;
};

// cp.parser<cpf_2_0>("7|7/7<7;8|8/8<8", ";", p2);
struct cpf_val_2 {
    cpf_val_2()
            : type(0)
            , molecular(0)
            , denominator(0)
            , threshold(0) {
    }

    bool is_valid() const {
        return type != 0 && molecular != 0 && denominator != 0 && threshold != 0;
    }
    
    int type;
    int molecular;
    int denominator;
    int threshold;
};

class cpf_2_0 {
public:
    cpf_2_0(cpf_val_2& cv) : cv2(cv) {
    }

    ~cpf_2_0() {
    }
    
    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        int i = 0;
        int length = strlen(str);
        std::string type = "0";
        if (!type.empty()) {
            cv2.type = atoi(type.c_str());
        }

        std::string molecular;
        for (; i < length && str[i] != '/'; ++i) {
            molecular += str[i];
        }

        if (!molecular.empty()) {
            cv2.molecular = atoi(molecular.c_str());
        }

        ++i;
        std::string denominator;
        for (; i < length && str[i] != '<'; ++i) {
            denominator += str[i];
        }

        if (!denominator.empty()) {
            cv2.denominator = atoi(denominator.c_str());
        }

        ++i;
        std::string threshold;
        for (; i < length; ++i) {
            threshold += str[i];
        }

        if (!threshold.empty()) {
            cv2.threshold = atoi(threshold.c_str());
        }

        return true;
    }
    
    cpf_val_2& cv2;
};

class cpf_2 {
public:
    cpf_2(std::map<int, cpf_val_2>* p = NULL)
            : _cfg(p) {
        
    }

    void set_cfg(std::map<int, cpf_val_2>* p) {
        _cfg = p;
    }

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        cpf_val_2 cv2;
        int i = 0;
        int length = strlen(str);
        std::string type;
        for (; i < length && str[i] != '|'; ++i) {
            type += str[i];
        }

        if (!type.empty()) {
            cv2.type = atoi(type.c_str());
        }

        ++i;
        std::string molecular;
        for (; i < length && str[i] != '/'; ++i) {
            molecular += str[i];
        }

        if (!molecular.empty()) {
            cv2.molecular = atoi(molecular.c_str());
        }

        ++i;
        std::string denominator;
        for (; i < length && str[i] != '<'; ++i) {
            denominator += str[i];
        }

        if (!denominator.empty()) {
            cv2.denominator = atoi(denominator.c_str());
        }

        ++i;
        std::string threshold;
        for (; i < length; ++i) {
            threshold += str[i];
        }

        if (!threshold.empty()) {
            cv2.threshold = atoi(threshold.c_str());
        }

        if (_cfg == NULL) {
            cfg.insert(std::pair<int, cpf_val_2>(cv2.type, cv2));
        } else {
            _cfg->insert(std::pair<int, cpf_val_2>(cv2.type, cv2));
        }
    
        return true;
    }
    
    std::map<int, cpf_val_2>   cfg;
    std::map<int, cpf_val_2>* _cfg;
};

// 3|20,14/14;6|8,14/14;8|5,9/9;
struct cpf_val_3 {
    cpf_val_3()
            : _type(0)
            , _ratio(0)
            , _min(0)
            , _max(0) {
    }
    
    int _type;
    int _ratio;
    int _min;
    int _max;
};

class cpf_3 {
public:
    cpf_3(std::map<int, cpf_val_3>* p = NULL)
            : _cfg(p) {
        
    }

    void set_cfg(std::map<int, cpf_val_3>* p) {
        _cfg = p;
    }

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        cpf_val_3 cv3;
        int i = 0;
        int length = strlen(str);
        std::string type;
        for (; i < length && str[i] != '|'; ++i) {
            type += str[i];
        }

        if (!type.empty()) {
            cv3._type = atoi(type.c_str());
        }

        ++i;
        std::string ratio;
        for (; i < length && str[i] != ','; ++i) {
            ratio += str[i];
        }

        if (!ratio.empty()) {
            cv3._ratio = atoi(ratio.c_str());
        }

        ++i;
        std::string min;
        for (; i < length && str[i] != '/'; ++i) {
            min += str[i];
        }

        if (!min.empty()) {
            cv3._min = atoi(min.c_str());
        }

        ++i;
        std::string max;
        for (; i < length; ++i) {
            max += str[i];
        }

        if (!max.empty()) {
            cv3._max = atoi(max.c_str());
        }

        if (_cfg == NULL) {
            cfg.insert(std::pair<int, cpf_val_3>(cv3._type, cv3));
        } else {
            _cfg->insert(std::pair<int, cpf_val_3>(cv3._type, cv3));
        }
    
        return true;
    }
    
    std::map<int, cpf_val_3>   cfg;
    std::map<int, cpf_val_3>* _cfg;
};
//技能激活条件(“*表示根据人物等级，表达式：*|数值”；“！表示根据美人等级，表达式：！|数值;”；“#表示根据神兵等级，表达式：#|数值;”）组合表达式为：!|1;#|1;
class cpf_4 {
public:
	cpf_4(std::map<std::string, int>* p = NULL)
		: _cfg(p) {

	}

	void set_cfg(std::map<std::string, int>* p) {
		_cfg = p;
	}

	bool operator() (const char* str)
	{
		if (str == NULL) {
			return false;
		}

		int i = 0;
		int length = strlen(str);
		std::string limit_type;
		for (; i < length && str[i] != '|'; ++i) {
			limit_type += str[i];
		}

		++i;
		std::string str_value;
		for (; i < length && str[i] != ';'; ++i) {
			str_value += str[i];
		}

		int limit_value = 0;
		if (!str_value.empty()) {
			limit_value = atoi(str_value.c_str());
		}

		if (_cfg == NULL) {
			cfg.insert(std::pair<std::string, int>(limit_type, limit_value));
		} else {
			_cfg->insert(std::pair<std::string, int>(limit_type, limit_value));
		}

		return true;
	}

	std::map<std::string, int>   cfg;
	std::map<std::string, int>* _cfg;
};

//x1_y1_x2_y2;
class cpf_5
{
public:
	cpf_5(std::vector<rect> *p = NULL)
		: _r(p) {

	}

	void set_cfg(std::vector<rect> *p) {
		_r = p;
	}

	bool operator() (const char* str)
	{
		if(NULL == str)
		{
			return false;
		}
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		sscanf(str, "%d_%d_%d_%d", &x1, &y1, &x2, &y2);
		if(_r != NULL)
		{
			_r->push_back(rect(x1, y1, x2, y2));
		}
		else
		{
			r.push_back(rect(x1, y1, x2, y2));
		}
		return true;
	}
	std::vector< rect > r;
	std::vector< rect >* _r;
};

// 4|x1_y1_x2_y2;\4|x1_y1_x2_y2
struct exp_region
{
public:
	exp_region():_rate(0) {}
public:
	int _rate;
	std::vector<rect> _r;
};

class cpf_6
{
public:
	cpf_6(std::vector<exp_region>* p = NULL)
		: _cfg(p) {
	}

	void set_cfg(std::vector<exp_region>* p) {
		_cfg = p;
	}

	bool operator() (const char* str)
	{
		if (str == NULL) {
			return false;
		}
		int i = 0;
		int length = strlen(str);
		exp_region er;
		for(; i < length && str[i] != '|'; ++i)
		{
			er._rate = str[i] - '0' + 10 * er._rate;
		}
		++i;
		cpf_5 cpf5;
		cpf5.set_cfg(&(er._r));
        cp.parser<cpf_5>(str + i, ";", cpf5);
		if(_cfg)
		{
			_cfg->push_back(er);
		}
		else
		{
			cfg.push_back(er);
		}
		return true;
	}

	std::vector<exp_region> cfg;
	std::vector<exp_region>* _cfg;
    cfg_parser cp;
};

// 3|20,14/14,!;6|8,14/14,@;8|5,9/9,$;
//3|20,14/14,!;6|8,14/14,@~10;8|5,9/9,$;
//

struct drop_content {
public:
    drop_content() : id(0), count(0), ratio_molecular(0), ratio_denominator(0), type(0), lv(0xff), to_type(0), timeout(0), job(5) {}
	void debug()
	{
		printf("id = %d, count = %d, ratio_molecular = %d, ratio_denominator = %d, type = %d, lv = %d, to_type = %d, timeout = %d, job = %d", id, count, ratio_molecular, ratio_denominator, type, lv, to_type, timeout, job);
		printf(",attr = ");
		for(size_t i = 0; i < attr.size(); ++i)
		{
			printf("%d ", attr[i]);
		}
		printf("\n");
	}
public:
    int id;
    int count;
    int ratio_molecular;
    int ratio_denominator;
    int type;
	int lv;		// 相差lv则不掉落
	int to_type; // 超时类型
	int timeout;
	int job;
	std::vector<int> attr;
};

class str2int 
{
public:
	int operator()(const char *str)
	{
//		int n = 0;
//		int ss = strlen(str);
//		for(int i = 0; i < ss; ++i)
//		{
//			n = n * 10 + str[i] - '0';
//		}
//		return n;
		return atoi(str);
	}
};

class date2int
{
public:
	int operator()(const char *str)
	{
		return time_utils::str2time(str);
	}
};

//n|n
class str2vec
{
public:
	vector<int> operator()(const string &str) 
	{
		vector<int> v;
		int i = 0; 
		int len = str.length();
		int n = 0;
		for(i = 0; i < len; ++i)
		{
			if(str[i] == '|')
			{
				v.push_back(n);
				n = 0;
				continue;
			}
			n = n * 10 + str[i] - '0';
		}
		v.push_back(n);
		return v;
	}
};

class cpf_drop_content {
public:
    cpf_drop_content(std::vector<drop_content>* p = NULL)
            : _cfg(p) {
        
    }

    void set_cfg(std::vector<drop_content>* p) {
        _cfg = p;
    }

	template<class T1, class T2>
		int assign(T1 **ptr, const string &str, T2 obj)
		{
			if(NULL == ptr || NULL == *ptr)
			{
				return -1;
			}
			**ptr = obj(str.c_str());
			return 0;
		}
    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }
        drop_content cv3;
		int i = 0;
		int length = strlen(str);
		for(; i < length && str[i] != '|'; ++i)
		{
			cv3.id = cv3.id * 10 + str[i] - '0';
		}
		++i;
		for(; i < length && str[i] != ','; ++i)
		{
			cv3.count = cv3.count * 10 + str[i] - '0';
		}
		++i;
		for(; i < length && str[i] != ',' && str[i] != ';'; ++i)
		{
			cv3.ratio_molecular = cv3.ratio_molecular * 10 + str[i] - '0';
		}
		cv3.ratio_denominator = 10000;
		void *ptr = NULL;
		std::string n;
		while(i <= length)
		{
			++i;
			if(i == length || ',' == str[i])
			{
				if(cv3.to_type == 1)
				{
					assign<int, date2int>((int **)&ptr, n, date2int());
				}
				else if(ptr == &cv3.attr)
				{
					assign<vector<int>, str2vec>((vector<int> **)&ptr, n, str2vec());
				}
				else
				{
					assign<int, str2int>((int **)&ptr, n, str2int());
				}
				n.clear();
				ptr = NULL;
				continue;
			}
			if('@' == str[i])
			{
				cv3.type = 1;
				ptr = NULL;
				n.clear();
				continue;
			}
			if('~' == str[i])
			{
				ptr = &cv3.lv;
				continue;
			}
			if('#' == str[i])
			{
				ptr = &cv3.to_type;
				continue;
			}
			if('!' == str[i])
			{
				assign<int, str2int>((int **)&ptr, n, str2int());
				ptr = &cv3.timeout;
				n.clear();
				continue;
			}
			if('$' == str[i])
			{
				ptr = &cv3.job;
				continue;
			}
			if('&' == str[i])
			{
				ptr = &cv3.attr;
				continue;
			}
			n += str[i];
		}
        if (_cfg == NULL) {
            cfg.push_back(cv3);
        } else {
            _cfg->push_back(cv3);
        }
    
        return true;
    }
    
    std::vector<drop_content>   cfg;
    std::vector<drop_content>* _cfg;
};


//110001_1_1$8B;110001_1_2$16B;
struct cast_equip {
	cast_equip()
		: equip_id(0)
		, cast_count(0)
		, improve_level(0)
		, probability(0)
	{
	}

	int equip_id;
	int cast_count;
	int improve_level;
	int probability;
};

class cpf_cast_equip {
public:
	cpf_cast_equip(std::multimap<int, cast_equip>* p = NULL)
		: _cfg(p) {

	}

	void set_cfg(std::multimap<int, cast_equip>* p) {
		_cfg = p;
	}

	bool operator() (const char* str)
	{
		if (str == NULL) {
			return false;
		}

		cast_equip cv3;
		int i = 0;
		int length = strlen(str);
		std::string equip_id;
		for (; i < length && str[i] != '_'; ++i) {
			equip_id += str[i];
		}

		if (!equip_id.empty()) {
			cv3.equip_id = atoi(equip_id.c_str());
		}

		++i;
		std::string cast_count;
		for (; i < length && str[i] != '_'; ++i) {
			cast_count += str[i];
		}

		if (!cast_count.empty()) {
			cv3.cast_count = atoi(cast_count.c_str());
		}

		++i;
		std::string improve_level;
		for (; i < length && str[i] != '$'; ++i) {
			improve_level += str[i];
		}

		if (!improve_level.empty()) {
			cv3.improve_level = atoi(improve_level.c_str());
		}

		++i;
		std::string probability;
		for (; i < length && str[i] != 'B'; ++i) {
			probability += str[i];
		}

		if (!probability.empty()) {
			cv3.probability = atoi(probability.c_str());
		}

		if (_cfg == NULL) {
			cfg.insert(std::pair<int, cast_equip>(cv3.equip_id, cv3));
		} else {
			_cfg->insert(std::pair<int, cast_equip>(cv3.equip_id, cv3));
		}

		return true;
	}

	std::multimap<int, cast_equip>   cfg;
	std::multimap<int, cast_equip>* _cfg;
};
//1_1_1;
struct succinct_info {
	succinct_info()
		: opt_type(0)
		, item_id(0)
		, item_count(0)
	{
	}

	int opt_type;
	int item_id;
	int item_count;
};

class cpf_succinct_info {
public:
	cpf_succinct_info(std::map<int, succinct_info>* p = NULL)
		: _cfg(p) {

	}

	void set_cfg(std::map<int, succinct_info>* p) {
		_cfg = p;
	}

	bool operator() (const char* str)
	{
		if (str == NULL) {
			return false;
		}

		succinct_info cv3;
		int i = 0;
		int length = strlen(str);
		std::string opt_type;
		for (; i < length && str[i] != '_'; ++i) {
			opt_type += str[i];
		}

		if (!opt_type.empty()) {
			cv3.opt_type = atoi(opt_type.c_str());
		}

		++i;
		std::string item_id;
		for (; i < length && str[i] != '_'; ++i) {
			item_id += str[i];
		}

		if (!item_id.empty()) {
			cv3.item_id = atoi(item_id.c_str());
		}

		++i;
		std::string item_count;
		for (; i < length && str[i] != '_'; ++i) {
			item_count += str[i];
		}

		if (!item_count.empty()) {
			cv3.item_count = atoi(item_count.c_str());
		}

		if (_cfg == NULL) {
			cfg.insert(std::pair<int, succinct_info>(cv3.opt_type, cv3));
		} else {
			_cfg->insert(std::pair<int, succinct_info>(cv3.opt_type, cv3));
		}

		return true;
	}

	std::map<int, succinct_info>   cfg;
	std::map<int, succinct_info>* _cfg;
};
struct gift {
	gift()
		: item_id(0)
		, profession(0)
		, count(0)
		, is_bind(0)
		, improve_level(0)
	{
	}

	int item_id;
	int profession;
	int count;
	int is_bind;
	int improve_level;
};

class cpf_gift {
public:
	cpf_gift(std::map<int, gift>* p = NULL)
		: _cfg(p) {

	}

	void set_cfg(std::map<int, gift>* p) {
		_cfg = p;
	}

	bool operator() (const char* str)
	{
		if (str == NULL) {
			return false;
		}

		gift cv3;
		int i = 0;
		int length = strlen(str);
		std::string item_id;
		for (; i < length && str[i] != '_'; ++i) {
			item_id += str[i];
		}

		if (!item_id.empty()) {
			cv3.item_id = atoi(item_id.c_str());
		}

		++i;
		std::string profession;
		for (; i < length && str[i] != '_'; ++i) {
			profession += str[i];
		}

		if (!profession.empty()) {
			cv3.profession = atoi(profession.c_str());
		}

		++i;
		std::string count;
		for (; i < length && str[i] != '_'; ++i) {
			count += str[i];
		}

		if (!count.empty()) {
			cv3.count = atoi(count.c_str());
		}

		++i;
		std::string is_bind;
		for (; i < length && str[i] != '_'; ++i) {
			is_bind += str[i];
		}

		if (!is_bind.empty()) {
			cv3.is_bind = atoi(is_bind.c_str());
		}
		++i;
		std::string improve_level;
		for (; i < length && str[i] != ';'; ++i) {
			improve_level += str[i];
		}

		if (!improve_level.empty()) {
			cv3.improve_level = atoi(improve_level.c_str());
		}

		if (_cfg == NULL) {
			cfg.insert(std::pair<int, gift>(cv3.item_id, cv3));
		} else {
			_cfg->insert(std::pair<int, gift>(cv3.item_id, cv3));
		}

		return true;
	}

	std::map<int, gift>   cfg;
	std::map<int, gift>* _cfg;
};

#endif // __CFG_PARSER_H__
