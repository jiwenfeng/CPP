#include "offline_role_mgr.h"
#include "config_mgr.h"

offline_role_mgr::offline_role_mgr()
{
    memset(roles, 0, sizeof(roles));

    head = NULL;
    tail = NULL;

    for (uint32 i = 0; i < MAX_OFFLINE_USER; ++i) {
        offline_role* ou = new offline_role;
        role_list.push(ou);
    }
}

offline_role_mgr::~offline_role_mgr()
{
    offline_role* ou;
    while (head != NULL) {
        ou = head->next[OPTE_LIST];
        if (head != NULL) {
            delete head;
        }

        head = ou;
    }

    tail = NULL;

    uint32 size = 0;
    while ((size = role_list.size()) > 0) {
        ou = role_list.top();
        role_list.pop();
        if (ou != NULL) {
            delete ou;
        }
    }
}

bool offline_role_mgr::replace(uint32 index, uint32 role_id, player_role* pr, center_role* cr)
{
    offline_role* ou = roles[index];
    while (ou != NULL){ // 查找替换原有数据
        if (ou->get_role_id() == pr->get_role_id()) {
            ou->attach(pr);
            ou->attach(cr);
            return true;
        }
        ou = ou->next[OPTE_CONFLICT];
    }

    return false;
}

bool offline_role_mgr::check_space()
{
    if (role_list.size() > 0) { // 查找空间，如果没有则释放
        return true;
    }
    
    if (head == NULL) {
        return false;
    }

    remove(head->get_role_id());
    return (role_list.size() > 0);
}

void offline_role_mgr::push(uint32 role_id, player_role* pr, center_role* cr)
{
    uint32 index = hash_index(role_id);
    if (replace(index, role_id, pr, cr)) {
        return;
    }

    if (!check_space()) {
        //printf("check_space has false\n");
        delete pr;
        delete cr;
        return;
    }

    offline_role* ou = roles[index]; // 经过删除节点,哈希表位置的表头可能发生变更,或者遍历,ou要从新指回哈希位置头节点
    offline_role* new_ou = role_list.top();
    role_list.pop();
    new_ou->attach(pr);
    new_ou->attach(cr);

    new_ou->next[OPTE_CONFLICT] = ou;
    if (ou != NULL) {
        ou->prev[OPTE_CONFLICT] = new_ou;
    }

    roles[index] = new_ou;

    if (tail == NULL) {
        head = tail = new_ou;
    } else {
        new_ou->prev[OPTE_LIST] = tail;
        tail->next[OPTE_LIST] = new_ou;
        tail = new_ou;
    }
}

int offline_role_mgr::pop(uint32 role_id, player_role*& pr, center_role*& cr)
{
    uint32 index = hash_index(role_id);
    offline_role* ou = roles[index];
    while (ou != NULL) {
        if (ou->get_role_id() == role_id) {
            break;
        }

        ou = ou->next[OPTE_CONFLICT];
    }

    if (ou == NULL) {
        return -1;
    }

    // 哈希链表头需要单独处理
    if (ou == roles[index]) {
        roles[index] = ou->next[OPTE_CONFLICT];
    }

    return pop_role(ou, pr, cr);
}

const offline_role* offline_role_mgr::get(uint32 role_id)
{
    return mutable_role(role_id);
}

offline_role* offline_role_mgr::mutable_role(uint32 role_id)
{
    uint32 index = hash_index(role_id);
    offline_role* ou = roles[index];
    while (ou != NULL) {
        if (ou->get_role_id() == role_id) {
            break;
        }

        ou = ou->next[OPTE_CONFLICT];
    }

    return ou;
}

int offline_role_mgr::remove(uint32 role_id)
{
    player_role* pr = NULL;
    center_role* cr = NULL; 
    pop(role_id, pr, cr);
    if (pr != NULL) {
        delete pr;
    }

    if (cr != NULL) {
        delete cr;
    }

    return 0;
}

void offline_role_mgr::remove_all()
{
    
}

void offline_role_mgr::reset_user(offline_role* ou)
{
    // 先处理哈希表的节点数据
    offline_role* prev = ou->prev[OPTE_CONFLICT];
    offline_role* next = ou->next[OPTE_CONFLICT];
    if (prev != NULL) {
        prev->next[OPTE_CONFLICT] = next;
    }
    if (next != NULL) {
        next->prev[OPTE_CONFLICT] = prev;
    }

    // 再处理list数据
    prev = ou->prev[OPTE_LIST];
    next = ou->next[OPTE_LIST];
    if (prev == NULL) {
        head = next;
    } else {
        prev->next[OPTE_LIST] = next;
    }
    if (next == NULL) {
        tail = prev;
    } else {
        next->prev[OPTE_LIST] = prev;
    }

    // 回收数据了
    ou->detach();
    ou->prev[OPTE_CONFLICT] = NULL;
    ou->prev[OPTE_LIST]     = NULL;
    ou->next[OPTE_CONFLICT] = NULL;
    ou->next[OPTE_LIST]     = NULL;
    role_list.push(ou);
}

int offline_role_mgr::pop_role(offline_role* ou, player_role*& pr, center_role*& cr)
{
    pr = ou->pop();
    cr = ou->pop_cr();
    reset_user(ou);
    return 0;
}
