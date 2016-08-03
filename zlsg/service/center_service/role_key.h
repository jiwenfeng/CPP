#ifndef __ROLE_KEY_H__
#define __ROLE_KEY_H__

class role_key {
public:
    role_key() {
        _key  = "";
        _time = 0;
    }

public:
    std::string _key;
    uint32      _time;
};

class role_key_mgr {
public:
    role_key_mgr();
    ~role_key_mgr();

public:
    std::string& operator[](uint32 i);

public:
    const void set_key(uint32 role_id, const std::string& key);
    const std::string& find(uint32 i) const;
    void erase(uint32 id);
    void add_key_time(uint32 id);
    void del_key_time(uint32 id);
    void update();

private:
    static std::string    _null;
    std::map<uint32, role_key> _keys;
    std::map<uint32, uint32>   _dels;
};

#endif // __ROLE_KEY_H__