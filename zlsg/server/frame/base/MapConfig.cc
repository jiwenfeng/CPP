#include "MapConfig.h"
#ifdef __LIB_MINI_SERVER__
#include "cocos2d.h"
#endif // __LIB_MINI_SERVER__

MapConfig_s::MapConfig_s():units(0)
{
}

MapConfig_s::~MapConfig_s()
{
    if(this->units)delete[] this->units;
}

bool MapConfig_s::isTransparent(float x,float y)
{
    int col = (int)x / this->unitGridWidth;
    int row = (int)y / this->unitGridHeight;
    int index = row*this->unitGridWidthNum + col;
    if(index >= this->totalUnitGridNum)return false;

    char mask = this->units[index];
    if(mask==MAP_UNIT_TRANSPARENT)return true;
	
    return false;
	
}

bool MapConfig_s::loadConfig(const char* filePath)
{
    char buf[256];
    sprintf(buf,"%s.scn",filePath);
#ifdef __LIB_MINI_SERVER__
    string fullNameScn;
	cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(buf, fullNameScn);
    bool ret1 = loadConfigScnData(fullNameScn.c_str());
#else
    bool ret1 = loadConfigScnData(buf);
#endif // __LIB_MINI_SERVER__

    sprintf(buf,"%s.data",filePath);
#ifdef __LIB_MINI_SERVER__
    string fullNameData;
	cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(buf, fullNameData);
    bool ret2 = loadConfigGridData(fullNameData.c_str());
#else
    bool ret2 = loadConfigGridData(buf);
#endif // __LIB_MINI_SERVER__

    return ret1&&ret2;
}
bool MapConfig_s::loadConfigScnData(const char* filePath)
{
    FILE* fp = fopen(filePath, "rb");
    if (!fp){
        return false;
    }
    fseek(fp,0,SEEK_END); 
    long len = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char* data = new char[len];
    fread(data, len, 1, fp);
    int* intData = (int*)data;
    //int version = *intData;
    //printf("version %d\n", version);
    intData++;
    int count = *intData;
    MapObject* objectBuf = (MapObject*)(data+sizeof(int)*2);
    /*int count = *((int*)data);
      MapObject* objectBuf = (MapObject*)(data+sizeof(int));*/
    for(int i=0;i<count;i++)
    {
        if(objectBuf->type == MAP_CONFIG_ACTOR_TYPE_NPC)
        {
            this->npcs.push_back(*objectBuf);
        }
        else if(objectBuf->type == MAP_CONFIG_ACTOR_TYPE_MONSTER)
        {
            this->monsters.push_back(*objectBuf);
        }
        else if (objectBuf->type == MAP_CONFIG_ACTOR_TYPE_PICK) {
            this->monsters.push_back(*objectBuf);
        }
        objectBuf++;
    }

    int* intBuf = (int*)objectBuf;
    birthPointX = *intBuf;
    intBuf++;
    birthPointY = *intBuf;

    fclose(fp);
    delete[] data;
    return true;
}
bool MapConfig_s::loadConfigGridData(const char* filePath)
{
    FILE* fp = fopen(filePath, "rb");
    if (!fp){
        return false;
    }

    fseek(fp,0,SEEK_END); 
    long len = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char* data = new char[len];
    fread(data, len, 1, fp);

    MapFileHead* head = (MapFileHead*)data;
    this->id = head->id;
    this->realWidth = head->width;
    this->realHeight = head->height;
    this->unitGridWidth = max(1, head->unitGridWidth);
    this->unitGridHeight = max(1, head->unitGridHeight);
    this->unitGridHeightNum = (this->realHeight + this->unitGridHeight -1)/this->unitGridHeight;
    this->unitGridWidthNum = (this->realWidth + this->unitGridWidth -1 )/this->unitGridWidth;
	
    this->totalUnitGridNum = unitGridHeightNum*unitGridWidthNum;
    this->units = new char[this->totalUnitGridNum];

    this->realWidth = (head->width + MAP_TILE_WIDTH-1) / MAP_TILE_WIDTH * MAP_TILE_WIDTH;
    this->realHeight = (head->height + MAP_TILE_HEIGHT -1) /MAP_TILE_HEIGHT * MAP_TILE_HEIGHT;

    /*this->realWidth = head->width / MAP_TILE_WIDTH * MAP_TILE_WIDTH;
      this->realHeight = head->height/MAP_TILE_HEIGHT * MAP_TILE_HEIGHT;*/

    this->tileWidthNum = (this->realWidth + MAP_TILE_WIDTH -1)/MAP_TILE_WIDTH;
    this->tileHeightNum = (this->realHeight + MAP_TILE_HEIGHT -1)/MAP_TILE_HEIGHT;
    this->width = this->tileWidthNum * MAP_TILE_WIDTH;
    this->height = this->tileHeightNum * MAP_TILE_HEIGHT;
    this->totalTileNum = this->tileHeightNum*this->tileWidthNum;
    memcpy(this->units,(data+sizeof(MapFileHead)),this->totalUnitGridNum);
    fclose(fp);
    delete[] data;
    return true;
}
