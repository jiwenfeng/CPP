#pragma once

#include <vector>
#include <map>

class CfgParser
{
public:
    CfgParser(void);
    virtual ~CfgParser(void);

public:
    template <typename T> 
    bool parser(const char* cstr, const char* delimiters, T& obj);
};

template <typename T> 
bool CfgParser::parser(const char* cstr, const char* delimiters, T& obj) {
    int len = strlen(cstr);
    if (len < 1) {
        return true;
    }

    const char* next = NULL;
    char *next_token = NULL;
    char* str = new char[len + 1];
    strcpy_s(str, len + 1, cstr);
    next = strtok_s(str, delimiters, &next_token);
    bool ret = false;
    while (next != NULL) {
        ret = obj(next);
        if (!ret) {
            return ret;
        }
        next = strtok_s(NULL, delimiters, &next_token);
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

    bool operator() (const char* str)
    {
        if (str == NULL) {
            return false;
        }

        cpf p;
        CfgParser cp;
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