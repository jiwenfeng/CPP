#include "randnpc_mgr.h"
#include "random.h"

randnpc_mgr::randnpc_mgr()
{
    last_fresh_time = 0;
    init();
}

randnpc_mgr::~randnpc_mgr()
{

}

void randnpc_mgr::init_ganlincun() //���ش����ˢ�ִ�,��4��
{
    client::randnpc* rn;

    rn = rns.add_rn();
    add_one(*rn, 198, 329, 3932, 2999); // ���ش����½�
    add_path(*rn, 3932, 2999);
    add_path(*rn, 3845, 3197);
    add_path(*rn, 3542, 3296);
    add_path(*rn, 3366, 3139);
    add_path(*rn, 3452, 2860);
    add_path(*rn, 3801, 2899);
    add_path(*rn, 3932, 2999);

    rn = rns.add_rn();
    add_one(*rn, 198, 329, 2578, 595); // ���ش����Ͻ�
    add_path(*rn, 2578, 595);
    add_path(*rn, 1968, 477);
    add_path(*rn, 2576, 753);
    add_path(*rn, 2971, 854);
    add_path(*rn, 3066, 710);
    add_path(*rn, 2578, 595);
    
    rn = rns.add_rn();
    add_one(*rn, 198, 329, 1833, 3556); // ���ش����²�
    add_path(*rn, 1833, 3556);
    add_path(*rn, 1616, 3495);
    add_path(*rn, 1833, 3556);


    rn = rns.add_rn();
    add_one(*rn, 198, 329, 174, 536); // ���ش������
    add_path(*rn, 174, 536);
    add_path(*rn, 524, 456);
    add_path(*rn, 786, 258);
    add_path(*rn, 1005, 635);
    add_path(*rn, 174, 536);

    rn = rns.add_rn();
    add_one(*rn, 198, 329, 2305, 2478); // ���ش�������Ů
    add_path(*rn, 2305, 2478);
    add_path(*rn, 2621, 2601);
    add_path(*rn, 2315, 2582);
    add_path(*rn, 2305, 2478);

    rn = rns.add_rn();
    add_one(*rn, 198, 329, 262, 3436); // ���ش����񾫹�
    add_path(*rn, 262, 3436);
    add_path(*rn, 305, 3535);
    add_path(*rn, 262, 3436);
    
}

void randnpc_mgr::init_yunwuchangshan() //�����ɽ���ˢ�ִ�,��1��
{
    client::randnpc* rn;

    rn = rns.add_rn();
    add_one(*rn, 199, 330, 2621, 615); // �����ɽ���½�
    add_path(*rn, 2621, 615);
    add_path(*rn, 2881, 773);
    add_path(*rn, 2621, 615);
   
    rn = rns.add_rn();
    add_one(*rn, 199, 330, 43, 317); // �����ɽ���Դ�
    add_path(*rn, 43, 317);
    add_path(*rn, 218, 278);
    add_path(*rn, 43, 317);
}

void randnpc_mgr::init_lyflg() //���Ƿ�������ˢ�ִ�,��1��
{
    client::randnpc* rn;

    rn = rns.add_rn();
    add_one(*rn, 220, 350, 2752, 913); // ���Ƿ����������
    add_path(*rn, 2752, 913);
    add_path(*rn, 2927, 794);
    add_path(*rn, 3277, 834);
    add_path(*rn, 3189, 993);
    add_path(*rn, 2930, 1111);
    add_path(*rn, 2752, 913);

    rn = rns.add_rn();
    add_one(*rn, 220, 350, 436, 1767); // ���Ƿ���ȳ��Ӥ�鴦
    add_path(*rn, 436, 1767);
    add_path(*rn, 349, 1966);
    add_path(*rn, 524, 2085);
    add_path(*rn, 436, 1767);
}

void randnpc_mgr::init_xms() //����ɽ���ˢ�ִ�,��1��
{
    client::randnpc* rn;

    rn = rns.add_rn();
    add_one(*rn, 289, 420, 873, 456); // ���Ƿ����������
    add_path(*rn, 873, 456);
    add_path(*rn, 480, 357);
    add_path(*rn, 436, 576);
    add_path(*rn, 742, 635);
    add_path(*rn, 873, 456);
}

void randnpc_mgr::init_mlgd() //ħ�������ˢ�ִ�,��1��
{
    client::randnpc* rn;

    rn = rns.add_rn();
    add_one(*rn, 290, 421, 568, 1231); // ħ���ع�������
    add_path(*rn, 568, 1231);
    add_path(*rn, 1048, 1132);
    add_path(*rn, 1267, 1350);
    add_path(*rn, 961, 1529);
    add_path(*rn, 568, 1231);
}

void randnpc_mgr::init_tianqicheng() //���������ˢ�ִ�,��16��
{
    client::randnpc* rn;

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 3932, 4707); // �����ǽ�鴦
    add_path(*rn, 3932, 4707);
    add_path(*rn, 3364, 4925);
    add_path(*rn, 3932, 5064);
    add_path(*rn, 3408, 5025);
    add_path(*rn, 2971, 5343);
    add_path(*rn, 3310, 5063);
    add_path(*rn, 3626, 4925);
    add_path(*rn, 3932, 4707);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 655, 119); // �����Ƕ�����
    add_path(*rn, 655, 119);
    add_path(*rn, 830, 238);
    add_path(*rn, 483, 396);
    add_path(*rn, 262, 695);
    add_path(*rn, 524, 417);
    add_path(*rn, 958, 100);
    add_path(*rn, 655, 119);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 524, 1966); // ����������
    add_path(*rn, 524, 1966);
    add_path(*rn, 873, 1807);
    add_path(*rn, 961, 1529);
    add_path(*rn, 873, 1847);
    add_path(*rn, 1308, 2084);
    add_path(*rn, 917, 2184);
    add_path(*rn, 524, 1966);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 3976, 278); // �������ϲ���Ȫ
    add_path(*rn, 3976, 278);
    add_path(*rn, 3845, 456);
    add_path(*rn, 4061, 594);
    add_path(*rn, 4459, 813);
    add_path(*rn, 3976, 873);
    add_path(*rn, 3932, 417);
    add_path(*rn, 3976, 278);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 8654, 2005); // ���������ϲ��������Ա�
    add_path(*rn, 8654, 2005);
    add_path(*rn, 8695, 2184);
    add_path(*rn, 8215, 2165);
    add_path(*rn, 8040, 2284);
    add_path(*rn, 8433, 2224);
    add_path(*rn, 8826, 1926);
    add_path(*rn, 8654, 2005);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 7909, 3058); // ��������̨��
    add_path(*rn, 7909, 3058);
    add_path(*rn, 8125, 3196);
    add_path(*rn, 7592, 3317);
    add_path(*rn, 8300, 3435);
    add_path(*rn, 8562, 3397);
    add_path(*rn, 8223, 3219);
    add_path(*rn, 7909, 3058);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 7909, 3058); // ��������̨��
    add_path(*rn, 7909, 3058);
    add_path(*rn, 8125, 3196);
    add_path(*rn, 7592, 3317);
    add_path(*rn, 8300, 3435);
    add_path(*rn, 8562, 3397);
    add_path(*rn, 8223, 3219);
    add_path(*rn, 7909, 3058);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 6557, 4748); // ����������
    add_path(*rn, 6557, 4748);
    add_path(*rn, 6773, 4727);
    add_path(*rn, 7035, 4806);
    add_path(*rn, 7253, 4906);
    add_path(*rn, 7037, 5043);
    add_path(*rn, 7294, 4965);
    add_path(*rn, 6950, 4847);
    add_path(*rn, 6557, 4748);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 7734, 1668); // ���������Ϸ�����
    add_path(*rn, 7734, 1668);
    add_path(*rn, 7472, 1708);
    add_path(*rn, 7428, 1807);
    add_path(*rn, 7384, 1867);
    add_path(*rn, 7647, 1787);
    add_path(*rn, 7690, 1728);
    add_path(*rn, 7734, 1668);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 219, 3562); // ����������
    add_path(*rn, 219, 3562);
    add_path(*rn, 655, 3773);
    add_path(*rn, 871, 3872);
    add_path(*rn, 614, 3991);
    add_path(*rn, 1267, 3694);
    add_path(*rn, 1002, 3775);
    add_path(*rn, 219, 3562);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 4413, 2780); // �������Ĵ�
    add_path(*rn, 4413, 2780);
    add_path(*rn, 4197, 2643);
    add_path(*rn, 4413, 2462);
    add_path(*rn, 4894, 2482);
    add_path(*rn, 4909, 2742);
    add_path(*rn, 4719, 2840);
    add_path(*rn, 4413, 2780);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 4413, 2780); // �������Ĵ�
    add_path(*rn, 4413, 2780);
    add_path(*rn, 4197, 2643);
    add_path(*rn, 4413, 2462);
    add_path(*rn, 4894, 2482);
    add_path(*rn, 4909, 2742);
    add_path(*rn, 4719, 2840);
    add_path(*rn, 4413, 2780);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 4413, 2780); // �������Ĵ�
    add_path(*rn, 4413, 2780);
    add_path(*rn, 4197, 2643);
    add_path(*rn, 4413, 2462);
    add_path(*rn, 4894, 2482);
    add_path(*rn, 4909, 2742);
    add_path(*rn, 4719, 2840);
    add_path(*rn, 4413, 2780);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 5068, 3912); // ���������Ĳ�
    add_path(*rn, 5068, 3912);
    add_path(*rn, 5287, 4012);
    add_path(*rn, 5112, 4131);
    add_path(*rn, 5068, 4548);
    add_path(*rn, 5285, 4686);
    add_path(*rn, 4765, 4331);
    add_path(*rn, 5068, 3912);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 5683, 5540); // ��������
    add_path(*rn, 5683, 5540);
    add_path(*rn, 6073, 5521);
    add_path(*rn, 6336, 5283);
    add_path(*rn, 6117, 5144);
    add_path(*rn, 5942, 5382);
    add_path(*rn, 5899, 5521);
    add_path(*rn, 5683, 5540);

    rn = rns.add_rn();
    add_one(*rn, 200, 331, 5593, 3714); // �������в�
    add_path(*rn, 5593, 3714);
    add_path(*rn, 5942, 3793);
    add_path(*rn, 6292, 3595);
    add_path(*rn, 6030, 3515);
    add_path(*rn, 5593, 3714);
}

void randnpc_mgr::init()
{
    init_lyflg();
    init_tianqicheng();
    init_xms();
    init_ganlincun();
    init_mlgd();
    init_yunwuchangshan();
}

void randnpc_mgr::add_one(client::randnpc& rn, uint32 map_id, uint32 scene_id, int x, int y)
{
    rn.set_role_id(1775);
    rn.set_map_id(map_id);
    rn.set_scene_id(scene_id);
    client::randnpc_postion* rp = rn.mutable_born_pos();
    rp->set_x(x);
    rp->set_y(y);
    rp->set_z(0);
}

void randnpc_mgr::add_path(client::randnpc& rn, int x, int y)
{
    client::randnpc_postion* rp = rn.add_path_rule();
    rp->set_x(x);
    rp->set_y(y);
    rp->set_z(0);
}

// ÿ���8�㣬12�㣬14�㣬16�㣬18�㣬20�㣬 22��
bool randnpc_mgr::is_need_fresh(uint32 now, uint32 hour)
{
    if (now - last_fresh_time < 3600){
        return false;
    }

    bool ret = (hour == 10 || hour == 12 || hour == 14 || hour == 16 || hour == 18 || hour == 20 || hour == 22);
    if (ret){
        last_fresh_time = now;
    }
    return ret;
}

client::randnpc* randnpc_mgr::get_pos()
{
    uint32 size = rns.rn_size();
    if (size <= 0){
        return NULL;
    }

    uint32 index = (mtrandom::rand_int32() % size);
    return rns.mutable_rn(index);
}
