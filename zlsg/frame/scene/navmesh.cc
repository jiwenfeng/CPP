#define __CC_STD_C_H__
#include <stdio.h>
#include "navmesh.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

#ifdef __LIB_MINI_SERVER__
#include "cocos2d.h"
#endif // __LIB_MINI_SERVER__

static const int NAVMESHSET_MAGIC = 'M'<<24 | 'S'<<16 | 'E'<<8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

struct navmesh_set_header
{
    int magic;
    int version;
    int numTiles;
    dtNavMeshParams params;
};

struct navmesh_tile_header
{
    dtTileRef tileRef;
    int dataSize;
};


navmesh_s::navmesh_s()
{
    handler = NULL;
    queryer = NULL;
}

navmesh_s::~navmesh_s()
{
    if (queryer != NULL){
        dtFreeNavMeshQuery(queryer);
    }

    if (handler != NULL){
        dtFreeNavMesh(handler);
    }
}

dtNavMesh* navmesh_s::load_scene_cfg(const char* file_path)
{
    FILE* fp = fopen(file_path, "rb");
    if (!fp){
        return NULL;
    }

    dtNavMesh* mesh = NULL;
    do{
        navmesh_set_header header;
        fread(&header, sizeof(navmesh_set_header), 1, fp);
        if (header.magic != NAVMESHSET_MAGIC || header.version != NAVMESHSET_VERSION){
            break;
        }

        mesh = dtAllocNavMesh();
        if (mesh == NULL){
            break;
        }

        dtStatus status = mesh->init(&header.params);
        if (dtStatusFailed(status)){
            dtFreeNavMesh(mesh);
            mesh = NULL;
            break;
        }

        for (int i = 0; i < header.numTiles; ++i){
            navmesh_tile_header tile_header;
            fread(&tile_header, sizeof(tile_header), 1, fp);
            if (!tile_header.tileRef || !tile_header.dataSize){
                break;
            }

            unsigned char* data = (unsigned char*)dtAlloc(tile_header.dataSize, DT_ALLOC_PERM);
            if (!data){
                break;
            }
            memset(data, 0, tile_header.dataSize);
            fread(data, tile_header.dataSize, 1, fp);
            mesh->addTile(data, tile_header.dataSize, DT_TILE_FREE_DATA, tile_header.tileRef, 0);
        }
    } while(false);

    fclose(fp);
    return mesh;
}

int navmesh_s::init(unsigned int scene_id)
{
    char file_path[MAX_FILE_PATH] = { 0 };
#ifdef __LIB_MINI_SERVER__
    snprintf(file_path, sizeof(file_path), "Map/data/%u/%u.bin", scene_id, scene_id);
    std::string fullNameScn = file_path;
    cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file_path, fullNameScn);
#else
    snprintf(file_path, sizeof(file_path), "./game_cfg/%u/%u.bin", scene_id, scene_id);
    std::string fullNameScn = file_path;
#endif  // __LIB_MINI_SERVER__

    handler = load_scene_cfg(fullNameScn.c_str());
    if (handler == NULL){
        return -1;
    }

    queryer = dtAllocNavMeshQuery();
    if (queryer->init(handler, NAVMESH_MAX_NODES) != DT_SUCCESS)
    {
        return -2;
    }

    return 0;
}

int navmesh_s::find_path(const map_postion& from, const map_postion& to, vector<move_map_postion>& path) const
{
    if (handler == NULL || queryer == NULL) {
        return -1;
    }

    const int MAX_MOVE_NODE = 128;
    dtPolyRef polys[MAX_MOVE_NODE];
    float straight[MAX_MOVE_NODE * 3];
    const float poly_pick_ext[3] = {2,4,2};
    int polys_count = 0;
    int straight_count[MAX_MOVE_NODE];
    float fpos[3] = {from.x / 10.0f, 0.0f, from.y / 10.0f};
    float tpos[3] = {to.x / 10.0f, 0.0f, to.y / 10.0f};
    dtQueryFilter filter;
    dtPolyRef fref, tref;

    if (queryer->findNearestPoly(fpos, poly_pick_ext, &filter, &fref, 0) != DT_SUCCESS
        || queryer->findNearestPoly(tpos, poly_pick_ext, &filter, &tref, 0) != DT_SUCCESS) {
            return -2;
    } 

    if (queryer->findPath(fref, tref, fpos, tpos, &filter, polys, &polys_count, MAX_MOVE_NODE) != DT_SUCCESS) {
        return -3;
    }

    if (queryer->findStraightPath(fpos, tpos, polys, polys_count, straight, 0, 0, straight_count, MAX_MOVE_NODE) != DT_SUCCESS) {
        return -4;
    }

    //printf("from:%d,%d,%d-->to:%d,%d,%d\n", from.x, from.y, from.z, to.x, to.y, to.z);
    float* pos = straight;
    path.resize(straight_count[0]);
    for (int i = straight_count[0] - 1; i >= 0; --i) {
        map_postion path_pos((*pos) * 10, (*(pos + 2)) * 10, (float)0);
        //printf("find_path node%d: %d %d %d\n", i, path_pos.x, path_pos.y, path_pos.z);
        path[i] = path_pos;
        pos += 3;
    }

    // 对寻出的路径进行修正
    if (path.size() < 2) {
        path.resize(2);
        path[0] = to;
        path[1] = from;
    } else {
        path[0] = to;
    }

    return 0;
}
