#include "config_mgr.h"
#include "family_skill.h"

//const uint32 family_skill::SKILL_ID[family_skill::SKILL_COUNT + 1] = {0, 34, 35, 36, 37, 38 , 39, 40, 41, 42, 43, 44, 45, 46};
const uint32 family_skill::SKILL_ID[family_skill::SKILL_COUNT] = {245, 255, 265, 275, 285, 295, 305, 315, 325, 330, 335, 340, 345};
const uint32 family_skill::UPGRADE_MONEY[2][family_skill::MAX_FAMILY_SKILL_LVL] = {
    {1000, 2000, 6400, 16000, 47900, 89400, 140600, 201300, 271700, 421100},
    {3100, 25500, 153000, 322000, 673800}
};

family_skill::family_skill(mysql::tb_family_skill* tfs/* = NULL*/)
    : _skill(tfs)
    , _change(false)
{
    writer_type = WOTE_NULL;

    if (_skill == NULL) {
        init(0);
    } else {
        parse(_skill->skills());
    }
}

family_skill::family_skill(uint32 family_id)
{
    init(family_id);
}

family_skill::~family_skill()
{
    if (_skill != NULL) {
        delete _skill;
        _skill = NULL;
    }
}

int family_skill::init(uint32 family_id)
{
    //writer_type = WOTE_INSERT;
    //_skill = new mysql::tb_family_skill;
    //_skill->set_family_id(family_id);
    //_change = true;

    //for (uint32 i = 0; i < SKILL_COUNT; ++i) {
    //    client::family_skill_info* fsi = _info.add_fsi();
    //    fsi->set_id(SKILL_ID[i]);
    //    fsi->set_lvl(1);
    //}
    //
    return 0;
}

void family_skill::clear()
{
    //_info.clear_fsi();
}

int family_skill::parse(const string& in)
{
    //clear();
    //
    //char* ptr     = (char*)in.c_str();
    //uint32 len    = in.size();
    //uint32 offset = 0;

    //while (offset < len) {
    //    uint16 msg_len = *(uint16*)(ptr + offset);
    //    offset += sizeof(uint16);
    //    client::family_skill_info* si = _info.add_fsi();
    //    if (!si->ParseFromArray(ptr + offset, msg_len)) {
    //        break;
    //    }

    //    offset += msg_len;
    //}
    
    return 0;
}

int family_skill::serialize(string& out)
{
    //char buf[MAX_SKILLS_BOLB_LEN];
    //uint32 offset = 0;
    //int32 size = _info.fsi_size();
    //for (int32 i = 0; i < size; ++i) {
    //    const client::family_skill_info& si = _info.fsi(i);
    //    uint32 msg_len = si.ByteSize();
    //    if (offset + sizeof(uint16) + msg_len > MAX_SKILLS_BOLB_LEN) {
    //        break;
    //    }

    //    *(uint16*)(buf + offset) = (uint16)msg_len;
    //    offset += sizeof(uint16);

    //    si.SerializeToArray((void*)(buf + offset), msg_len);
    //    offset += msg_len;
    //}

    //out.append(buf, offset);

    return 0;
}

int family_skill::save(amtsvr_context* ctx)
{
    if (!_change || _skill == NULL) {
        return -1;
    }

    mysql::tb_family_skill* tfs = new mysql::tb_family_skill(*_skill);
    CACHE_KEY k(tfs->family_id(), 0);
    string out;
    serialize(out);
    tfs->set_skills(out);
    write(ctx, k, tfs);
    _change = false;
    writer_type = WOTE_NULL;

    return 0;
}

void family_skill::remove()
{
    if (writer_type == WOTE_INSERT) {
        _change = false;
    } else {
        writer_type = WOTE_REMOVE;
        _change = true;
    }
}

int family_skill::upgrade(uint32 id)
{
//     int32 size = _info.fsi_size();
//     for (int32 i = 0; i < size; ++i) {
//         client::family_skill_info* si = _info.mutable_fsi(i);
//         if (si->id() != id) {
//             continue ;
//         }
//         
//         const mysql::tb_skill* ts = CONFMGR->get_sys_skill(si->id());
//         if (ts == NULL) {
//             break;
//         }
//         
//         uint32 next = (uint32)atoi(ts->costom_id().c_str());
//         if (next == 0) {
//             break;
//         }
//             
//         si->set_id(next);
//         si->set_lvl(si->lvl() + 1);
//         _change = true;
//         writer_type = WOTE_UPDATE;
//         
//         return next;
//     }
    
    return 0;
}

//int family_skill::get_lvl(uint32 id)
//{
//    int32 size = _info.fsi_size();
//    for (int32 i = 0; i < size; ++i) {
//        const client::family_skill_info& si = _info.fsi(i);
//        if (si.id() == id) {
//            return si.lvl();
//        }
//    }
//
//    return 0;
//}

//int family_skill::fill_skills(client::family_skills_rsp& rsp)
//{
//    rsp.mutable_fsi()->CopyFrom(_info.fsi());
//
//    return 0;
//}

//int family_skill::fill_skill(uint32 skill_id, client::family_skill_info* fsi)
//{
//    int32 size = _info.fsi_size();
//    for (int32 i = 0; i < size; ++i) {
//        const client::family_skill_info& si = _info.fsi(i);
//        if (si.id() == skill_id) {
//            fsi->CopyFrom(si);
//            return 0;
//        }
//    }
//
//    return -1;
//}

uint32 family_skill::id_to_index(uint32 id)
{
	//int32 size = _info.fsi_size();
	//for (int32 i = 0; i < size; ++i) {
	//	const client::family_skill_info& si = _info.fsi(i);
	//	if (si.id() == id) {
	//		return i + 1;
	//	} 
	//}

	return 0;
}

