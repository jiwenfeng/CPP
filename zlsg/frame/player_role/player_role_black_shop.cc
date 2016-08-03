#include "player_role_black_shop.h"
#include "random.h"
#include "config_treasure.h"

player_role_black_shop::player_role_black_shop(mysql::tb_role_black_shop* b) : db_role_black_shop(b)
{

}

player_role_black_shop::~player_role_black_shop()
{
    if (db_role_black_shop != NULL) {
        delete db_role_black_shop;
        db_role_black_shop = NULL;
    }
}

int player_role_black_shop::set_db_black_shop(mysql::tb_role_black_shop* b)
{
	if (b == NULL) {
		return -1;
	}

	if (db_role_black_shop == b) {
		return 0;
	}

	if (db_role_black_shop != NULL) {
		delete db_role_black_shop;
	}

	db_role_black_shop = b;
	parse_black_shop(db_role_black_shop->black_shops());

    return 0;
}

int player_role_black_shop::save_black_shop(uint32 role_id, amtsvr_context* ctx)
{
	string out;
	int count = serialize_black_shop(out);
	if (count <= 0) {
		if (db_role_black_shop == NULL) {
			return 0;
		}
		writer_type = WOTE_REMOVE;
	} else {
		if (db_role_black_shop == NULL) {
			db_role_black_shop = new mysql::tb_role_black_shop;
			db_role_black_shop->set_role_id(role_id);
			writer_type = WOTE_INSERT;
		} else {
			writer_type = WOTE_UPDATE;
		}
		db_role_black_shop->set_black_shops(out);
	}

	mysql::tb_role_black_shop* trb = new mysql::tb_role_black_shop(*db_role_black_shop);
	CACHE_KEY k(trb->role_id(), 0);
	write(ctx, k, trb);

    return 0;
}

int player_role_black_shop::attach_role_black_shop(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
	if (pb_count <= 0){
		return 0;
	}

	for (int i = 0; i < pb_count; ++i){
		if (i == 0){
			set_db_black_shop(static_cast<mysql::tb_role_black_shop*>(pb_ctx[i].pb_msg));
		} else {
			delete pb_ctx[i].pb_msg;
		}
	}

	return 0;
}

bool player_role_black_shop::reflesh_shop(uint32 base_denominator,std::map<uint32,client::shop_info> new_black_shop_map_)
{
	// 先清空原来的数据
	remove_all();
	// 刷N个格子，填充数据
	uint32 slot_count = CONFMGR->get_basic_value(10009);
	for (uint32 i = 0; i < slot_count; ++i){
		int rand_value = static_cast<int>(mtrandom::rand_mid32(0,base_denominator));
		//递增数
		int attenuation = 0;
		std::map<uint32, client::shop_info>::const_iterator it_1 = new_black_shop_map_.begin();
		if (new_black_shop_map_.size() == 0){
			break;
		}

		for (;it_1 != new_black_shop_map_.end();++it_1){
			attenuation += it_1->second.probability();
			if (rand_value > attenuation){
				continue;
			}
			
			client::black_shop_db_info data_info;
			data_info.set_item_slot(i);
			data_info.set_item_id(it_1->second.item_id());
			data_info.set_item_count(it_1->second.item_count());
			data_info.set_key_id(it_1->second.key_id());
			// 插入商店列表
			add_blacl_shop_data(data_info);
			break;
		}
	}

	return true;
}