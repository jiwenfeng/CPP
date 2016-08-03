#include "player_role_ext_attr.h"
#include "player_role_ext_attr_family.h"

void role_attr_add(client::role_attr& a, const client::role_attr& b)
{
    const google::protobuf::Reflection* ref_attr = a.GetReflection();
    const google::protobuf::Descriptor* des_attr = a.GetDescriptor();
    const google::protobuf::Reflection* ref = b.GetReflection();
    const google::protobuf::Descriptor* des = b.GetDescriptor();

    for (int32 index = 0; index < des->field_count(); ++index) {
        switch (des->field(index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 val = ref_attr->GetUInt32(a, des_attr->field(index));
            uint32 add = ref->GetUInt32(b, des->field(index));
            ref_attr->SetUInt32(&a, des_attr->field(index), val + add);
            break;
                                                             }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint64 val = ref_attr->GetUInt64(a, des_attr->field(index));
            uint64 add = ref->GetUInt64(b, des->field(index));
            ref_attr->SetUInt64(&a, des_attr->field(index), val + add);
            break;
                                                             }
        default: break;
        }
    }
}

void role_attr_sub(client::role_attr& a, const client::role_attr& b)
{
    const google::protobuf::Reflection* ref_attr = a.GetReflection();
    const google::protobuf::Descriptor* des_attr = a.GetDescriptor();
    const google::protobuf::Reflection* ref = b.GetReflection();
    const google::protobuf::Descriptor* des = b.GetDescriptor();

    for (int32 index = 0; index < des->field_count(); ++index) {
        switch (des->field(index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 val = ref_attr->GetUInt32(a, des_attr->field(index));
            uint32 add = ref->GetUInt32(b, des->field(index));
            val = val > add ? val - add : 0;
            ref_attr->SetUInt32(&a, des_attr->field(index), val);
            break;
                                                             }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint64 val = ref_attr->GetUInt64(a, des_attr->field(index));
            uint64 add = ref->GetUInt64(b, des->field(index));
            val = val > add ? val - add : 0;
            ref_attr->SetUInt64(&a, des_attr->field(index), val);
            break;
                                                             }
        default: break;
        }
    }
}

player_role_ext_attr::player_role_ext_attr()
{
    db_role_ext_attr = NULL;
}

player_role_ext_attr::~player_role_ext_attr()
{
    if (db_role_ext_attr != NULL) {
        delete db_role_ext_attr;
        db_role_ext_attr = NULL;
    }
}

int player_role_ext_attr::set(uint32 role_id, uint32 src_type, const client::role_attr& attr)
{
    if (db_role_ext_attr == NULL) {
        db_role_ext_attr = new mysql::tb_role_ext_attr();
        db_role_ext_attr->set_role_id(role_id);
        db_role_ext_attr->set_src_type(src_type);
        writer_type = WOTE_INSERT;
    }

    _attr.CopyFrom(attr);

    return 0;
}

int player_role_ext_attr::add(uint32 role_id, uint32 src_type, const client::role_attr& attr)
{
    if (db_role_ext_attr == NULL) {
        db_role_ext_attr = new mysql::tb_role_ext_attr();
        db_role_ext_attr->set_role_id(role_id);
        db_role_ext_attr->set_src_type(src_type);
        writer_type = WOTE_INSERT;
    }

    role_attr_add(_attr, attr);

    return 0;
}

int player_role_ext_attr::sub(uint32 role_id, uint32 src_type, const client::role_attr& attr)
{
    if (db_role_ext_attr == NULL) {
        db_role_ext_attr = new mysql::tb_role_ext_attr();
        db_role_ext_attr->set_role_id(role_id);
        db_role_ext_attr->set_src_type(src_type);
        writer_type = WOTE_INSERT;
    }

    role_attr_sub(_attr, attr);

    return 0;
}

int player_role_ext_attr::attach(mysql::tb_role_ext_attr* p)
{
    if (p == db_role_ext_attr) {
        return 0;
    }

    if (p == NULL) {
        return -1;
    }

    if (db_role_ext_attr != NULL) {
        delete db_role_ext_attr;
        db_role_ext_attr = NULL;
    }

    db_role_ext_attr = p;
    _attr.ParseFromString(db_role_ext_attr->attr());

    return 0;
}

void player_role_ext_attr::save(amtsvr_context* ctx)
{
    if (db_role_ext_attr == NULL) {
        return;
    }

    db_role_ext_attr->set_attr(_attr.SerializeAsString());

    CACHE_KEY k(db_role_ext_attr->role_id(), db_role_ext_attr->src_type());
    mysql::tb_role_ext_attr* tmp = new mysql::tb_role_ext_attr(*db_role_ext_attr);
    write(ctx, k, tmp);
    writer_type = WOTE_UPDATE;
}

const client::role_attr& player_role_ext_attr::get_attr()
{
    return _attr;
}

void player_role_ext_attr::clear()
{
    _attr.Clear();
}

///////////////////////////////////////////////////////////////////////////

player_role_ext_attr_mgr::player_role_ext_attr_mgr()
{
    ext_attrs.resize(REASTE_MAX);
    _recalc = true;
}

player_role_ext_attr_mgr::~player_role_ext_attr_mgr()
{
    for (size_t i = 0; i < ext_attrs.size(); ++i) {
        if (ext_attrs[i] != NULL) {
            delete ext_attrs[i];
        }
    }

    ext_attrs.clear();
}

int player_role_ext_attr_mgr::attach(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        mysql::tb_role_ext_attr* a = static_cast<mysql::tb_role_ext_attr*>(pb_ctx[i].pb_msg);
        if (a == NULL) {
            continue ;
        }

        player_role_ext_attr* p = NULL;
        if (a->src_type() == REASTE_FAMILY) {
            p = new player_role_ext_attr_family;
            p->attach(a);
            ext_attrs[i] = p;
        } else {
            p = new player_role_ext_attr;
            p->attach(a);
            ext_attrs[i] = p;
        }
    }

    return 0;
}

int player_role_ext_attr_mgr::add_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr)
{
    if (src_type >= REASTE_MAX || src_type < 0) {
        return -1;
    }

    _recalc = true;
    if (ext_attrs[src_type] == NULL && create(role_id, src_type, attr) == 0) {
        return 0;
    }

    return ext_attrs[src_type]->add(role_id, src_type, attr);
}

int player_role_ext_attr_mgr::sub_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr)
{
    if (src_type >= REASTE_MAX || src_type < 0) {
        return -1;
    }

    _recalc = true;
    if (ext_attrs[src_type] == NULL && create(role_id, src_type, attr) == 0) {
        return 0;
    }

    return ext_attrs[src_type]->sub(role_id, src_type, attr);
}

int player_role_ext_attr_mgr::set_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr)
{
    if (src_type >= REASTE_MAX || src_type < 0) {
        return -1;
    }

    _recalc = true;
    if (ext_attrs[src_type] == NULL && create(role_id, src_type, attr) == 0) {
        return 0;
    }

    return ext_attrs[src_type]->set(role_id, src_type, attr);
}

player_role_ext_attr* player_role_ext_attr_mgr::get(ROLE_EXT_ATTR_SRC_TYPE_E src_type, uint32 role_id)
{
    if (src_type >= REASTE_MAX || src_type < 0) {
        return NULL;
    }

    client::role_attr attr;
    if (ext_attrs[src_type] == NULL && create(role_id, src_type, attr) != 0) {
        return NULL;
    }

    return ext_attrs[src_type];
}

const client::role_attr& player_role_ext_attr_mgr::get_all_attr()
{
    if (!_recalc) {
        return _attr;
    }

    _attr.Clear();
    for (uint32 i = 0; i < REASTE_MAX; ++i) {
        if (ext_attrs[i] != NULL) {
            role_attr_add(_attr, ext_attrs[i]->get_attr());
        }
    }

    _recalc = false;
    return _attr;
}

void player_role_ext_attr_mgr::save(amtsvr_context* ctx)
{
    for (uint32 i = 0; i < REASTE_MAX; ++i){
        if (ext_attrs[i] != NULL) {
            ext_attrs[i]->save(ctx);
        }
    }
}

int player_role_ext_attr_mgr::create(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr)
{
    if (role_id == 0 || src_type >= REASTE_MAX) {
        return -1;
    }

    player_role_ext_attr* p = NULL;
    if (src_type == REASTE_FAMILY) {
        p = new player_role_ext_attr_family;
    } else {
        p = new player_role_ext_attr;
    }

    p->set(role_id, src_type, attr);
    ext_attrs[src_type] = p;

    return 0;
}

int player_role_ext_attr_mgr::clear_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type)
{
    if (role_id == 0 || src_type >= REASTE_MAX) {
        return -1;
    }

    _recalc = true;
    client::role_attr attr;
    if (ext_attrs[src_type] == NULL && create(role_id, src_type, attr) != 0) {
        return NULL;
    }

    //for (size_t i = 0; i < ext_attrs.size(); ++i) {
        ext_attrs[src_type]->clear();
    //}

    return 0;
}
