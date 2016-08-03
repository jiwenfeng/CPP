#include <stdlib.h>
#include "BinTable.h"

#ifdef __LIB_MINI_SERVER__
#pragma warning(disable: 4251)
#include "cocos2d.h"
#include "CCFileUtils.h"
using namespace cocos2d;
#endif // __LIB_MINI_SERVER__

/*static TTable*	getSingleGroupTable( const char* pszTableName )
{
    if( !pszTableName )
        return NULL;
    TTableGroup_s* g = BinTable_s::GetTalbeGroup( pszTableName );
    if( !g )
        return NULL;

    return g->GetTable(pszTableName);
}

static int splitString(const string& str, vector<string>& ret_, string sep)
{
    if (str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}*/

//string replace(const string& str, const string& src, const string& dest)
//{
//	string ret;
//
//	string::size_type pos_begin = 0;
//	string::size_type pos       = str.find(src);
//	while (pos != string::npos)
//	{
//		//cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
//		ret.append(str.data() + pos_begin, pos - pos_begin);
//		ret += dest;
//		pos_begin = pos + 1;
//		pos       = str.find(src, pos_begin);
//	}
//	if (pos_begin < str.length())
//	{
//		ret.append(str.begin() + pos_begin, str.end());
//	}
//	return ret;
//}
BinTable_s BinTable_s::bin;

BinTable_s::BinTable_s(void)
{
}

BinTable_s::~BinTable_s(void)
{
    for (size_t i=0; i<m_tableGroups.size(); i++)
    {
        delete m_tableGroups[i];
    }

}

TTableGroup_s* BinTable_s::GetTalbeGroup( const string& fileName )
{
    for (size_t i=0; i<bin.m_tableGroups.size(); i++)
    {
        if ( bin.m_tableGroups[i]->m_fileName == fileName )
        {
            return bin.m_tableGroups[i];
        }
    }

    // Load
    TTableGroup_s* tableGroup = new TTableGroup_s();

    tableGroup->Load(fileName);

    bin.m_tableGroups.push_back(tableGroup);

    return tableGroup;
}

void BinTable_s::clear()
{
    bin.m_tableGroups.clear();
}


// 
// class:TTable
//
int TTable::GetField( int id, int filedID )
{
    if ( id <= 0 ) return 0;
    if ( id > m_recordCount ) return 0;

    if ( filedID < 0 ) return 0;	
    if ( filedID >= m_filedCount ) return 0;

    int index = filedID + m_filedCount*(id-1);
    return m_data[index];
}

int TTable::GetField( int id, const string& filedName )
{
    int filedID = GetFiledIndex(filedName);
    return GetField(id, filedID);
}

string TTable::GetFieldString( int id, int filedID )
{
    int data =  GetField(id, filedID);

    if ( data > 0 )
    {
        return m_tableGroup->m_strKey[data-1];	
    }	

    return "";
}

string TTable::GetFieldString( int id, const string& filedName )
{
    int data =  GetField(id, filedName);

    if ( data > 0 )
    {
        if (m_tableGroup->m_strKey.size() <= (size_t)(data - 1)) {
            char buf[32] = {0};
            sprintf(buf, "%d", data);
            printf("%s:%d, Warning cfg table_name = %s, id = %d, filedname = %s, data = %d\n", __FILE__, __LINE__, m_name.c_str(), id, filedName.c_str(), data);
            return string(buf);
        }

        return m_tableGroup->m_strKey[data-1];	
    }	

    return "";
}

bool TTable::GetOneColumFileds( const string& attrName, map<int, int>& attrs )
{
    attrs.clear();

    int id = GetFiledIndex( attrName );
    if ( 0 > id) return false;

    // int size = m_recordCount * m_filedCount;
    for (int i = 0; i < m_recordCount; i ++)
    {
        int offset = id + i * m_filedCount;
        attrs.insert(map<int, int>::value_type(m_data[offset], id));
    }

    //if(attrs.size() < 0) return false;

    return true;
}

bool TTable::FindRecords( const string& filedName, int value, vector<int>& records )
{
    records.clear();

    int id = GetFiledIndex( filedName );

    if ( id < 0 ) return false;

    int size = m_recordCount * m_filedCount;
    for (int i=id; i<size; i+= m_filedCount )
    {
        if ( m_data[i] == value )
        {
            records.push_back(i/m_filedCount + 1);
        }
    } 

    if ( records.size() <= 0 ) return false;

    return true;
}

int TTable::GetFiledIndex( const string& filedName )
{
    int id = m_tableGroup->GetStringKey(filedName);

    if ( id > 0 )
    {
        for (size_t i=0; i<m_fieldstrKey.size(); i++)
        {
            if ( m_fieldstrKey[i] == id )
            {
                return i;
            }
        } 
    }

    return -1;	
}

TTable* TTableGroup_s::GetTable(int index)
{
    return m_tables[index];
}

TTable* TTableGroup_s::GetTable(const string& tableName)
{
    for (size_t i=0; i<m_tables.size(); i++)
    {
        if ( m_tables[i]->m_name == tableName )
        {
            return m_tables[i];
        }
    }

    return NULL;
}

TTableGroup_s::TTableGroup_s()
{

}

TTableGroup_s::~TTableGroup_s()
{
    for (size_t i=0; i<m_tables.size(); i++)
    {
        delete m_tables[i];
    }
}

int TTableGroup_s::GetStringKey( const string& str )
{
    for (size_t i=0; i<m_strKey.size(); i++)
    {
        if ( m_strKey[i] == str )
        {
            return (i+1);
        }
    }


    return -1;
}

void TTableGroup_s::Load(const string& name)
{
    m_fileName = name;

#ifdef __LIB_MINI_SERVER__
    string fileName = "Data/" + m_fileName + ".str";
    string dataName = "Data/" + m_fileName + ".bin";
#else
    string fileName = "./game_cfg/" + name + ".str";
    string dataName = "./game_cfg/" + name + ".bin";
#endif // __LIB_MINI_SERVER__

#ifdef __LIB_MINI_SERVER__
    string fullName;
	CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName.c_str(), fullName);
    unsigned long len;
    char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullName.c_str(), "rb", &len);
    unsigned long size = len;
#else
    ifstream infile(fileName.c_str(), ifstream::binary);
    if (!infile.is_open()) {
        return ;
    }

    infile.seekg(0, ifstream::end);
    unsigned long size = (unsigned long)infile.tellg();
    if (size == 0) {
        infile.close();
        return ;
    }

    char* data = (char*)malloc(sizeof(char) * size);
    memset(data, 0, sizeof(char) * size);
    infile.seekg(0);
    infile.read(data, size);
    infile.close();
#endif // __LIB_MINI_SERVER__

    unsigned long pos = 3;
    int startPos = 3;
    while( pos < size )
    {
        if ( data[pos] == '^' )
        {
            data[pos] = '\0';
            m_strKey.push_back(&data[startPos]);

            startPos = pos + 1;
        }
        pos++;
    }	

    free(data);

#ifdef __LIB_MINI_SERVER__
    CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(dataName.c_str(), fullName);
    data = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullName.c_str(), "rb", &len);
    size = len;
#else 
    infile.open(dataName.c_str(), ifstream::binary);
    if (!infile.is_open()) {
        return ;
    }

    infile.seekg(0, ifstream::end);
    size = (long)infile.tellg();
    if (size == 0) {
        infile.close();
        return ;
    }

    data = (char*)malloc(sizeof(char) * size);
    memset(data, 0, sizeof(char) * size);
    infile.seekg(0);
    infile.read(data, size);
    infile.close();
#endif // __LIB_MINI_SERVER__

    int* endPtr  = (int*)(data + size);
    int* curPtr  = (int*)(data);
    int nRecordIndex = 0;
    while( curPtr < endPtr )
    {
        TTable* table = new TTable();
        m_tables.push_back(table);		
        table->m_tableGroup = this;

        table->m_name = m_strKey[*curPtr-1];
        curPtr++;
        if (curPtr >= endPtr) {
            continue;
        }

        table->m_filedCount = *curPtr;
        curPtr++;
        if (curPtr >= endPtr) {
            continue;
        }

        table->m_recordCount  = *curPtr;
        curPtr++;
        if (curPtr >= endPtr) {
            continue;
        }

        for (int i=0; i<table->m_filedCount; i++)
        {
            table->m_fieldstrKey.push_back(*curPtr);
            curPtr++;
        }

        int dataSize = 0;
        dataSize = table->m_filedCount * table->m_recordCount;
        if (dataSize == 0) {
            continue;
        }

        table->m_data = (int*)malloc(sizeof(int) * dataSize);
        memset(table->m_data, 0, sizeof(int) * dataSize);
        memcpy(table->m_data, curPtr,  sizeof(int) * dataSize);

        curPtr += dataSize;

        ++nRecordIndex;
    }	

    //if( nRecordIndex >= 2 )
    //{
    //	int n = 0;
    //}

    free(data);
}
