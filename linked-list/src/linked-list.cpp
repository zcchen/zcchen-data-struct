#include "zcchen/linked-list.hpp"

template<typename T>
LinkedList::LinkedList_t::~LinkedList_t()
{
    destroy_obj(&obj);
    size = 0;
}

template<typename T>
LinkedList(obj_create_func create_func, obj_destroy_func destroy_func) :
    m_create_func(create_func),
    m_destroy_func(destroy_func)
{
}

template<typename T>
LinkedList::~LinkedList()
{
    while (m_linked_list) {
        auto obj = m_linked_list;
        m_linked_list = m_linked_list.next;
        delete obj;
    }
}

