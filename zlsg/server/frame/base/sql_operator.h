#ifndef __SQL_OPERATOR_H__
#define __SQL_OPERATOR_H__
#include "define.h"

#include <mysql.h>
#include <map>
#include <string>
#include <fstream>
 #include <stdlib.h>


class record_set
{
public:
    inline record_set( MYSQL_RES* p ) :mres(p)
    {
        fiedls = NULL;
        fields_num = mysql_num_fields(mres);
        fiedls = mysql_fetch_fields(mres);
        for (uint32 i = 0; i < fields_num; ++i){
            std::string name = fiedls[i].name;
            map_index.insert(std::pair<std::string, uint32>(name, i)); 
        }
    }

	virtual ~record_set()
    {
        mysql_free_result(mres);
    }

	bool next_row()
    {
	    m_stRow = mysql_fetch_row(mres);
        row_len = mysql_fetch_lengths(mres);
	    return ( m_stRow != NULL );
    }

    const char* index(std::string& name, uint32* pos = NULL)
    {
        std::map<std::string, uint32>::iterator it = map_index.find(name);
        if (it == map_index.end()){
            return NULL;
        }

        uint32 i = it->second;
        if (mres == NULL || i >= fields_num){
            return NULL;
        }

        if (pos != NULL){
            *pos = i;
        }

        return m_stRow[i];
    }

    bool get(std::string& name, uint8& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val =  (uint8)atoi(p);
        return true;
    }

    bool get(std::string& name, int8& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (int8)atoi(p);
        return true;
    }

    bool get(std::string& name, uint16& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (uint16)atoi(p);
        return true;
    }

    bool get(std::string& name, int16& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (int16)atoi(p);
        return true;
    }

    bool get(std::string& name, uint32& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (uint32)strtoul(p, NULL, 10);
        return true;
    }

    bool get(std::string& name, int& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (int)atoi(p);
        return true;
    }

    bool get(std::string& name, uint64& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (uint64)strtoul(p, NULL, 10);
        return true;
    }

    bool get(std::string& name, int64& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (int64)strtoul(p, NULL, 10);
        return true;
    }

    bool get(std::string& name, std::string& sval)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        sval = p;
        return true;
    }

    bool get(std::string& name, double& dVal)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        dVal = (double)atof(p);
        return true;
    }

    bool get(std::string& name, float& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (float)atof(p);
        return true;
    }

    bool get(std::string& name, bool& val)
    {
        const char *p = index(name);
        if (p == NULL){
            return false;
        }

        val = (atoi(p) != 0);
        return true;
    }

    bool get_blob(std::string& name, std::string& val)
    {
        uint32 pos = 0; 
        const char *p = index(name, &pos);
        if (p == NULL){
            return false;
        }

        val.append(p, row_len[pos]);
        return true;
    }

private:
	uint32           fields_num;
	MYSQL_ROW          m_stRow; 
    MYSQL_FIELD*       fiedls;
	MYSQL_RES*         mres;
    unsigned long *    row_len;
    std::map<std::string, uint32> map_index;
};

class database
{
public:
    database()
    {
    }

    database(const char* host, const char* user, const char* pass, const char* db, short port = 3306)
    {
        set_param(host, user, pass, db, port);
    }

    virtual ~database()
    {
        close();
    }

    void set_param(const char* host, const char* user, const char* pass, const char* db, short port = 3306)
    {
        mhost = host;
        muser = user;
        mpass = pass;
        mdb = db;
        mport = port;
        mmysql = NULL;
    }

    int open();
    void close();
    bool is_open();
    bool check();
    record_set* query(const char* sql, my_ulonglong& rows, int iCommit = 1);

    uint32 get_errno();

private:
    std::string mhost;
    std::string muser;
    std::string mpass;
    std::string mdb;
    short       mport;

    MYSQL* mmysql;
};


#endif
