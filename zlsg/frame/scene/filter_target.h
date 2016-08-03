#ifndef __FILTER_TARGET_H__
#define __FILTER_TARGET_H__

class filter_target_null
{
public:
    virtual bool operator()(scene_role* psr) = 0;
};

class filter_target
{
public:
    filter_target();
    ~filter_target();

public:
    void filter_target_by_atk_mode(scene_role* psr, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends);
};

#endif // __FILTER_TARGET_H__