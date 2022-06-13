#include "zcchen/descriptor.h"
#include <stdlib.h>
#include <stdio.h>

class Abc
{
public:
    Abc() :
        m_a(1),
        m_b(2),
        m_c(3)
    {}
    ~Abc(){}

    int getA() {
        return m_a;
    }
    int getB() {
        return m_b;
    }
    int getC() {
        return m_c;
    }
private:
    int m_a;
    int m_b;
    int m_c;
};

void* abc_create(uint32_t* allocate_size, void* param_data, uint32_t param_size, void* context)
{
    void* r = new Abc(); // TODO: looks memory leaks here, but I cannot find it why.
    if (r) {
        *allocate_size = sizeof(Abc);
        return r;
    }
    *allocate_size = 0;
    return nullptr;
}

void abc_destroy(void** object, void* context)
{
    delete ((Abc*)(*object));
    *object = nullptr;
}

struct zcchen_descriptor_t* d = NULL;

int main(void)
{
    int32_t max_size = 3;
    printf("--------------- creating --------------\n");
    d = zcchen_descriptor_create(max_size, abc_create, abc_destroy);
    if (!d) {
        fprintf(stderr, "ERROR, cannot allocate descriptor...\n");
        return 1;
    }
    printf("--------------- allocating ------------\n");
    for (int i = 0; i < max_size + 1; ++i) {
        int r = zcchen_descriptor_add(d, NULL, 0);
        printf(">>> allocate descriptor %d\n", r);
    }
    printf("--------- getting the objects --------\n");
    for (int i = 1; i < max_size + 1; ++i) {
        uint32_t obj_size;
        auto obj_ptr = (Abc*)zcchen_descriptor_getObj(d, i, &obj_size);
        printf("obj_ptr: %p, obj_size: %d\n", obj_ptr, obj_size);
        printf("obj_ptr->a: %d\n", obj_ptr->getA());
        printf("obj_ptr->b: %d\n", obj_ptr->getB());
        printf("obj_ptr->c: %d\n", obj_ptr->getC());
    }
    printf("--------------- removing --------------\n");
    for (int i = 0; i < max_size; ++i) {
        zcchen_descriptor_remove(d, i);
    }
    printf("--------------- destroying ------------\n");
    if (d) {
        zcchen_descriptor_destroy(&d);
    }
    return 0;
}
