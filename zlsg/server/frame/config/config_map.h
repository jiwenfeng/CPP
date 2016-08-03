#ifndef __config_map_H__
#define __config_map_H__

#include "cfg_cache_mgr.h"
#include "config_base.h"
#include "config_raid.h"

#define DEFAULT_GRID_WIDTH 48

class config_map : public MapConfig_s {
public:
    config_map();
    ~config_map(void);

public:
    inline const uint32 get_id                  () const { return id               ; }
    inline const uint32 get_width               () const { return width            ; }
    inline const uint32 get_height              () const { return height           ; }
    inline const uint32 get_real_width          () const { return realWidth        ; }
    inline const uint32 get_real_height         () const { return realHeight       ; }
    inline const uint32 get_unit_grid_width     () const { return unitGridWidth    ; }
    inline const uint32 get_unit_grid_height    () const { return unitGridHeight   ; }
    inline const uint32 get_unit_grid_width_num () const { return unitGridWidthNum ; }
    inline const uint32 get_unit_grid_height_num() const { return unitGridHeightNum; }
    inline const uint32 get_birth_point_x       () const { return birthPointX      ; }
    inline const uint32 get_birth_point_y       () const { return birthPointY      ; }

    inline const std::vector<MapObject>& get_npcs()     const { return npcs    ; }
    inline const std::vector<MapObject>& get_monsters() const { return monsters; }
    
public:
    const bool   is_block(int x, int y) const;
    const bool   unit_mask(int x, int y, char mask) const;
    const uint32 get_region(int x, int y) const;

public:

};

class config_map_mgr : public config_base_mgr
{
public:
	config_map_mgr(std::map<std::string, MapConfig_s*>* p = NULL);
	virtual ~config_map_mgr(void);

public:
    virtual int init(TTableGroup_s* p);
    int init(std::map<std::string, MapConfig_s*>* p);
    const std::map<uint32, config_map*>& get_config_map() const { return _cm; }
    const config_map* get_config_map(uint32 id) const;

private:
    std::map<uint32, config_map*> _cm;
};

#endif // __config_map_H__
