#ifndef __NAVMESH_H__
#define __NAVMESH_H__

#include "postion.h"

#define NAVMESH_MAX_NODES 2048 

class dtNavMesh;
class dtNavMeshQuery;

class navmesh_s
{
public:
    navmesh_s();

    virtual ~navmesh_s();

    int init(unsigned int scene_id);

    int find_path(const map_postion& from, const map_postion& to, std::vector<move_map_postion>& path) const;

protected:
    dtNavMesh* load_scene_cfg(const char* file_path);

private:
    dtNavMesh* handler;
    dtNavMeshQuery* queryer;
};

#endif
