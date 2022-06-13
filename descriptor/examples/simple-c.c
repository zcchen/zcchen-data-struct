#include "zcchen/descriptor.h"
#include <stdlib.h>
#include <stdio.h>

struct Abc_t {
    int a;
    int b;
    int c;
};

void* abc_create(uint32_t* allocate_size, void* param_data, uint32_t param_size, void* context)
{
    if (param_data || param_size != 0) {
        fprintf(stderr, "WARNING, the param_data nor param_size is NOT supported yet.\n");
    }
    void* r = malloc(sizeof(struct Abc_t));
    if (!r) {
        fprintf(stderr, "ERROR, cannot allocate struct Abc_t object.\n");
        *allocate_size = 0;
        return NULL;
    }
    ((struct Abc_t*)r)->a = 1;
    ((struct Abc_t*)r)->b = 2;
    ((struct Abc_t*)r)->c = 3;
    *allocate_size = sizeof(struct Abc_t);
    return r;
}

void abc_destroy(void** object, void* context)
{
    if (*object) {
        free(*object);
        *object = NULL;
    }
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
        struct Abc_t* obj_ptr = (struct Abc_t*)zcchen_descriptor_getObj(d, i, &obj_size);
        printf("obj_ptr: %p, obj_size: %d\n", obj_ptr, obj_size);
        printf("obj_ptr->a: %d\n", obj_ptr->a);
        printf("obj_ptr->b: %d\n", obj_ptr->b);
        printf("obj_ptr->c: %d\n", obj_ptr->c);
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
