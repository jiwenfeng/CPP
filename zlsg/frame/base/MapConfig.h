#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <memory.h>
using namespace std;
#define MAP_TILE_WIDTH 256 //地图瓦片像素宽度
#define MAP_TILE_HEIGHT 256 //地图瓦片像素高度
#define MAP_UNIT_BLOCK 1 //阻挡
#define MAP_UNIT_TRANSPARENT 2 //透明
#define MAP_CONFIG_ACTOR_TYPE_MONSTER 0//怪
#define MAP_CONFIG_ACTOR_TYPE_NPC 1
#define MAP_CONFIG_ACTOR_TYPE_PICK 3
#pragma pack(push)
#pragma pack(1)
struct _MapObject
{
    int id;
    char type;
    char dir;
    int x;
    int y;
};
#pragma pack(pop)
typedef struct _MapObject MapObject;

struct _MapFileHead
{
    int id;
    int width;
    int height;
    int unitGridWidth;
    int unitGridHeight;
};
typedef struct _MapFileHead MapFileHead;

class MapConfig_s
{
public:
    MapConfig_s();
    ~MapConfig_s();
    bool loadConfig(const char* file_path);
    bool isTransparent(float x,float y);

private:
    bool loadConfigGridData(const char* filePath);
    bool loadConfigScnData(const char* filePath);

public:
    int id;
    int width;             // 地图像素宽度，被网格化后的
    int height;            // 地图像素高度
    int realWidth;         // 真实宽度
    int realHeight;           
    int unitGridWidth;     // 地图单元格宽度
    int unitGridHeight;    // 地图单元格高度
    int unitGridWidthNum;  // x方向地图单元格总数
    int unitGridHeightNum; // y方向地图单元格总数
    int totalUnitGridNum;  // 总单元格数量
    int tileWidthNum;      // x地图切割后的瓦片数量
    int tileHeightNum;     // y地图切割后的瓦片数量
    int totalTileNum;
    int birthPointX;       // 出生点坐标
    int birthPointY;       
    std::vector<MapObject> objects;
    char *units;                 // 单个格数组，保存阻挡信息
    vector<MapObject> npcs;      // 场景npc列表
    vector<MapObject> monsters;  // 场景上所有怪列表
};
