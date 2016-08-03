#ifndef __PB_CONTAINER_H__
#define __PB_CONTAINER_H__

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <string> 

template<typename T>
class pb_container
{
public:
    pb_container(T* pb);

    pb_container();

    virtual ~pb_container();

    int attach(T* pb);

    T* detach();

    const T* get_pb() const;

    T* mutable_pb();

protected:
    T* _pb;
};

template<typename T>
pb_container<typename T>::pb_container(T* pb) : _pb(pb)
{
}

template<typename T>
pb_container<typename T>::pb_container() : _pb(NULL)
{
}

template<typename T>
pb_container<typename T>::~pb_container()
{
    if (_pb != NULL){
        delete _pb;
        _pb = NULL;
    }
}

template<typename T>
int pb_container<typename T>::attach(T* pb)
{
    if (_pb != NULL){
        return -1;
    }

    _pb = pb;
}

template<typename T>
T* pb_container<typename T>::detach()
{
    T* tmp = _pb;
    _pb = NULL;
    return tmp;
}

template<typename T>
const T* pb_container<typename T>::get_pb() const
{
    return _pb;
}

template<typename T>
T* pb_container<typename T>::mutable_pb()
{
    return _pb;
}

#endif
