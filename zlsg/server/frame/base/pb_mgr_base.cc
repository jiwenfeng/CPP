#include "pb_mgr_base.h"

int pb_mgr_base::base_init()
{
    pb_obj* obj = NULL;
    obj = new pb_obj(&mpb_tb_role_mail, "", "select * from tb_role_mail where role_id=%s;", "tb_role_mail");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_mail.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_pet, "", "select * from tb_role_pet where role_id=%s;", "tb_role_pet");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_pet.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_task, "", "select * from tb_role_task where role_id=%s;", "tb_role_task");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_task.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_task_chain, "", "select * from tb_role_task_chain where role_id=%s;", "tb_role_task_chain");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_task_chain.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_label, "", "select * from tb_role_label where role_id=%s;", "tb_role_label");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_label.GetTypeName(), obj));
    obj->add_key("role_id");
    obj->add_key("label_code");

    obj = new pb_obj(&mpb_tb_role_center_set, "", "select * from tb_role_center_set where role_id=%s;", "tb_role_center_set");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_center_set.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_explore, "", "select * from tb_role_explore where role_id=%s;", "tb_role_explore");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_explore.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_explore_detail, "", "select * from tb_role_explore_detail where role_id=%s;", "tb_role_explore_detail");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_explore_detail.GetTypeName(), obj));
    obj->add_key("id");
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_statis, "", "select * from tb_role_statis where role_id=%s;", "tb_role_statis");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_statis.GetTypeName(), obj));
    obj->add_key("role_id");
    obj->add_key("statis_type");

    obj = new pb_obj(&mpb_tb_role_set, "", "select * from tb_role_set where role_id=%s;", "tb_role_set");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_set.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_order, "", "select * from tb_role_order where role_id=%s;", "tb_role_order");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_order.GetTypeName(), obj));
    obj->add_key("role_id");
    obj->add_key("store_id");

    obj = new pb_obj(&mpb_tb_role_ext_attr, "", "select * from tb_role_ext_attr where role_id=%s;", "tb_role_ext_attr");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_ext_attr.GetTypeName(), obj));
    obj->add_key("role_id");
    obj->add_key("src_type");

    obj = new pb_obj(&mpb_tb_player, "", "select * from tb_player where login='%s';", "tb_player");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_player.GetTypeName(), obj));
    obj->add_key("id");

    obj = new pb_obj(&mpb_tb_player_role, "", "select * from tb_player_role where role_id=%s;", "tb_player_role");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_player_role.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_status, "", "select * from tb_role_status where role_id=%s;", "tb_role_status");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_status.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_skill, "", "select * from tb_role_skill where role_id=%s;", "tb_role_skill");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_skill.GetTypeName(), obj));
    obj->add_key("role_id");
    obj->add_key("smy_id");

    obj = new pb_obj(&mpb_tb_role_buffer, "", "select * from tb_role_buffer where role_id=%s;", "tb_role_buffer");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_buffer.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_props, "", "select * from tb_role_props where role_id=%s;", "tb_role_props");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_props.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_raid, "", "select * from tb_role_raid where role_id=%s;", "tb_role_raid");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_raid.GetTypeName(), obj));
    obj->add_key("role_id");
    obj->add_key("raid_id");

    obj = new pb_obj(&mpb_tb_role_auto_raid, "", "select * from tb_role_auto_raid where role_id=%s;", "tb_role_auto_raid");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_auto_raid.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_mounts, "", "select * from tb_role_mounts where role_id=%s;", "tb_role_mounts");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_mounts.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_potential, "", "select * from tb_role_potential where role_id=%s;", "tb_role_potential");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_potential.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_cd_cooldown, "", "select * from tb_role_cd_cooldown where role_id=%s;", "tb_role_cd_cooldown");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_cd_cooldown.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_treasure, "", "select * from tb_role_treasure where role_id=%s;", "tb_role_treasure");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_treasure.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_target, "", "select * from tb_role_target where role_id=%s;", "tb_role_target");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_target.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_vip, "", "select * from tb_role_vip where role_id=%s;", "tb_role_vip");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_vip.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_lottery, "", "select * from tb_role_lottery where role_id=%s;", "tb_role_lottery");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_lottery.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_black_shop, "", "select * from tb_role_black_shop where role_id=%s;", "tb_role_black_shop");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_black_shop.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_levels, "", "select * from tb_role_levels where role_id=%s;", "tb_role_levels");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_levels.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_gem, "", "select * from tb_role_gem where role_id=%s;", "tb_role_gem");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_gem.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_bows, "", "select * from tb_role_bows where role_id=%s;", "tb_role_bows");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_bows.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_cycle, "", "select * from tb_role_cycle where role_id=%s;", "tb_role_cycle");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_cycle.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_devil, "", "select * from tb_role_devil where role_id=%s;", "tb_role_devil");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_devil.GetTypeName(), obj));
    obj->add_key("role_id");

    return 0;
}
