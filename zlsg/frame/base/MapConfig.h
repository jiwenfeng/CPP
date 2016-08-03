#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <memory.h>
using namespace std;
#define MAP_TILE_WIDTH 256 //��ͼ��Ƭ���ؿ��
#define MAP_TILE_HEIGHT 256 //��ͼ��Ƭ���ظ߶�
#define MAP_UNIT_BLOCK 1 //�赲
#define MAP_UNIT_TRANSPARENT 2 //͸��
#define MAP_CONFIG_ACTOR_TYPE_MONSTER 0//��
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
    int width;             // ��ͼ���ؿ�ȣ������񻯺��
    int height;            // ��ͼ���ظ߶�
    int realWidth;         // ��ʵ���
    int realHeight;           
    int unitGridWidth;     // ��ͼ��Ԫ����
    int unitGridHeight;    // ��ͼ��Ԫ��߶�
    int unitGridWidthNum;  // x�����ͼ��Ԫ������
    int unitGridHeightNum; // y�����ͼ��Ԫ������
    int totalUnitGridNum;  // �ܵ�Ԫ������
    int tileWidthNum;      // x��ͼ�и�����Ƭ����
    int tileHeightNum;     // y��ͼ�и�����Ƭ����
    int totalTileNum;
    int birthPointX;       // ����������
    int birthPointY;       
    std::vector<MapObject> objects;
    char *units;                 // ���������飬�����赲��Ϣ
    vector<MapObject> npcs;      // ����npc�б�
    vector<MapObject> monsters;  // ���������й��б�
};
