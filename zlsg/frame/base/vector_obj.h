#ifndef __VECTOR_OBJ_H__
#define __VECTOR_OBJ_H__

#define ADD_SIZE_PER 10  // 每次增加50个空位

template<typename T, uint32 INIT_SIZE>
class vector_obj
{
public:
    vector_obj()
    {
        _size = INIT_SIZE;
        _count = 0;
        objs = new T*[_size];
    }

    ~vector_obj()
    {
        if (objs != NULL){
            for (uint32 i = 0; i < _count; ++i){
                if (objs[i] != NULL){
                    delete objs[i];
                }
            }
            delete[] objs;
            objs = NULL;
        }
    }


    int push_back(const T& v)
    {
        if (_count >= _size){
            _size += ADD_SIZE_PER;
            T** tmp = new T*[_size];;
            memcpy(tmp, objs, sizeof(T*)*_count);
            delete[] objs;
            objs = tmp;
        }

        T* new_val = new T;
        *new_val = v;
        objs[_count++] = new_val;
        
        return 0;
    }

    inline uint32 size()
    {
        return _count;
    }
    
    const T& operator[](uint32 i) const
    {
        return *(objs[i]);
    }

private:
    uint32 _size;
    uint32 _count;
    T** objs;
};

#endif
