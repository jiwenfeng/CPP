#ifndef __BIN_TABLE_S_H__
#define __BIN_TABLE_S_H__

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <memory.h>

using namespace std;

/*static int splitString(const string& str, vector<string>& ret_, string sep = ",");
static TTable*	getSingleGroupTable( const char* pszTableName );*/

struct TTable;
struct TTableGroup_s;

/*
func: loadAllGroupTable()
Description: load all config tables and for the checking OF tasks,
call this function while initialize the app.
*/

struct TTable
{
    friend struct TTableGroup_s;

    bool FindRecords( const string& filedName, int value, vector<int>& recordIds );
    bool GetOneColumFileds( const string& attrName, map<int, int>& attrs );	

    int GetField( int id, int filedID );	
    int GetField( int id, const string& filedName );

    string GetFieldString( int id, int filedID );	
    string GetFieldString( int id, const string& filedName );	

    int GetFiledIndex( const string& filedName );

    TTable() {
        m_filedCount = 0;
        m_recordCount = 0;
        m_id = 0;
        m_data = NULL;
        m_tableGroup = NULL;
    }

    ~TTable() {
        if (m_data != NULL) {
            free(m_data);
        }
    }

public:
    int  m_filedCount;
    int  m_recordCount;

private:
    int m_id;
    int* m_data;
    TTableGroup_s* m_tableGroup;

    vector<int>  m_fieldstrKey;

    string m_name;
};

struct TTableGroup_s
{
    friend struct TTable;

    TTable* GetTable(const string& filedName);
    TTable* GetTable(int index);

    int GetStringKey( const string& str );

    void Load(const string& fileName);

    TTableGroup_s();
    ~TTableGroup_s();

private:
    vector<TTable*> m_tables;
    vector<string>  m_strKey;

public:
    string m_fileName;
};

class BinTable_s
{
public:
    BinTable_s(void);
    ~BinTable_s(void);

    static TTableGroup_s* GetTalbeGroup( const string& fileName );
    static void clear();

private:
    vector<TTableGroup_s*> m_tableGroups;
    static BinTable_s bin;
};

#endif // __BIN_TABLE_S_H__
