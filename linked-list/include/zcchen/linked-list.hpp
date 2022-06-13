#ifndef ZCCHEN_LINKED_LIST_HPP
#define ZCCHEN_LINKED_LIST_HPP

#include <cstdint>
#include <cstdlib>

#include "zcchen/object.h"

template<typename T>
class LinkedList
{
public:
    LinkedList(obj_create_func create_func, obj_destroy_func destroy_func);
    virtual ~LinkedList();

    // push obj to the last.
    int push(T& obj, uint32_t size = sizeof(T));

    // pop the obj from first and delete it.
    T& pop(uint32_t* size);

    // push obj to the last.
    int insert(T& ref_obj, T& obj, uint32_t size = sizeof(T));

    // return the first object
    T& first(uint32_t* size);

    // return the last object
    T& last(uint32_t* size);

private:
    struct LinkedList_t {
        T obj = 0;
        uint32_t size = 0;
        struct LinkedList_t *next = nullptr;
        struct LinkedList_t *prev = nullptr;
        ~LinkedList_t();
    };

    // the wrapper to create / destroy the LinkedList_t obj.
    int create_obj(T* obj);
    void destroy_obj(T* obj);

    struct LinkedList_t *m_linked_list = nullptr;
    const obj_create_func m_create_func;
    const obj_destroy_func m_destroy_func
};

#endif /* ifndef ZCCHEN_LINKED_LIST_HPP */
