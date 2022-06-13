#include "zcchen/descriptor.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct descriptor__link_list_t {
    int32_t index;
    void* obj_ptr;
    uint32_t obj_size;
    struct descriptor__link_list_t* next;
    struct descriptor__link_list_t* prev;
};

struct zcchen_descriptor_t {
    struct {
        // max size will be hold.
        uint32_t max_size;
        // the constructor and desstructor
        obj_create_func create_func;
        obj_destroy_func destroy_func;
    } params;

    struct {
        uint32_t last_index;
        uint32_t allocated_size;
    } vars;

    struct descriptor__link_list_t* data;
};

void local_descriptor_remove_obj(struct zcchen_descriptor_t *t, struct descriptor__link_list_t **d)
{
    struct descriptor__link_list_t* tmp = *d;
    if (!tmp) {
        return;
    }
    if (tmp->obj_ptr) {
        t->params.destroy_func(&(tmp->obj_ptr), NULL);
        tmp->obj_ptr = NULL;
    }
    tmp->obj_size = 0;
    if (tmp->prev) {
        tmp->prev->next = tmp->next;
    }
    if (tmp->next) {
        tmp->next->prev = tmp->prev;
    }
    free(*d);
    *d = NULL;
}

CONFIGAPI struct zcchen_descriptor_t* CONFIGCALL
zcchen_descriptor_create(int32_t max_size, obj_create_func create_func, obj_destroy_func destroy_func)
{
    if (!create_func) {
        fprintf(stderr, "ERROR, the create_func <%p> is invalid.\n", create_func);
        return NULL;
    }
    if (!destroy_func) {
        fprintf(stderr, "ERROR, the destroy_func <%p> is invalid.\n", destroy_func);
        return NULL;
    }
    struct zcchen_descriptor_t* r = malloc(sizeof(struct zcchen_descriptor_t));
    if (!r) {
        int errno_last = errno;
        fprintf(stderr, "ERROR, cannot malloc zcchen_descriptor_t: %s\n", strerror(errno_last));
        return NULL;
    }
    r->params.max_size = max_size;
    r->params.create_func = create_func;
    r->params.destroy_func = destroy_func;
    r->vars.last_index = 0;
    r->vars.allocated_size = 0;
    r->data = NULL;
    return r;
}

CONFIGAPI void CONFIGCALL
zcchen_descriptor_destroy(struct zcchen_descriptor_t **descriptor)
{
    struct zcchen_descriptor_t* t = *descriptor;
    if (!t) {       // descriptor is NULL.
        return;
    }
    while (t->data) {
        struct descriptor__link_list_t *n = t->data->next;
        local_descriptor_remove_obj(*descriptor, &(t->data));
        t->data = n;
    }
    free(*descriptor);
    *descriptor = NULL;
}

CONFIGAPI int32_t CONFIGCALL
zcchen_descriptor_add(struct zcchen_descriptor_t* descriptor, void* param_data, uint32_t param_size)
{
    // check the descriptor* pointer
    int32_t ret = -1;
    if (!descriptor) {
        fprintf(stderr, "ERROR, the descriptor is : %p\n", descriptor);
        return ret;
    }
    // check the allocated size
    if (descriptor->vars.allocated_size >= descriptor->params.max_size) {
        fprintf(stderr, "ERROR, the descriptor is reaching the max.\n");
        ret = -2;
        return ret;
    }
    // the function vars
    struct descriptor__link_list_t* d = NULL;
    void* obj_ptr = NULL;
    uint32_t obj_size = 0;
    // create & check the obj
    obj_ptr = descriptor->params.create_func(&obj_size, param_data, param_size, NULL);
    if (!obj_ptr || obj_size == 0) {
        fprintf(stderr, "ERROR, failed to create the obj via pre-set create_func.\n");
        ret = -3;
        goto CLEAN;
    }
    // create & check the linked list data.
    d = malloc(sizeof(struct descriptor__link_list_t));
    if (!d) {
        int errno_last = errno;
        fprintf(stderr, "ERROR, cannot malloc the linked obj under zcchen_descriptor: %s\n",
                strerror(errno_last));
        ret = -4;
        goto CLEAN;
    }
    d->obj_ptr = obj_ptr;
    d->obj_size = obj_size;
    d->next = NULL;
    d->prev = NULL;
    d->index = descriptor->vars.last_index + 1;
    // setup the prev / next for obj d.
    if (!descriptor->data) {    // link to the first if descriptor->data is empty.
        descriptor->data = d;
    }
    else {  // find the last node to be appended.
        struct descriptor__link_list_t* prev = descriptor->data;
        while (prev->next) {
            prev = prev->next;
        }
        prev->next = d;
        d->prev = prev;
    }
    // normal return
    descriptor->vars.last_index ++;
    descriptor->vars.allocated_size ++;
    ret = descriptor->vars.last_index;
CLEAN:
    if (ret <= 0) {
        if (obj_ptr) {
            free(obj_ptr);
            obj_ptr = NULL;
        }
        obj_size = 0;
        if (d) {
            free(d);
            d = NULL;
        }
    }
    return ret;
}

CONFIGAPI int CONFIGCALL
zcchen_descriptor_remove(struct zcchen_descriptor_t* descriptor, int32_t index)
{
    // check the descriptor* pointer
    int32_t ret = -1;
    if (!descriptor) {
        fprintf(stderr, "ERROR, the descriptor is : %p\n", descriptor);
        return ret;
    }
    // find the obj via the id;
    struct descriptor__link_list_t* d = descriptor->data;
    while (d) {
        if (d->index == index) {
            break;
        }
        d = d->next;
    }
    if (!d) {   // cannot find that index
        return 1;
    }
    else {  // remove it.
        if (d == descriptor->data) {    // removing the 1st data
            struct descriptor__link_list_t* n = d->next;
            local_descriptor_remove_obj(descriptor, &d);
            descriptor->data = n;
        }
        else {
            local_descriptor_remove_obj(descriptor, &d);
        }
        descriptor->vars.allocated_size --;
        return 0;
    }
}

CONFIGAPI void* CONFIGCALL
zcchen_descriptor_getObj(struct zcchen_descriptor_t* descriptor, int32_t index, uint32_t* obj_size)
{
    // check the descriptor* pointer
    if (!descriptor) {
        fprintf(stderr, "ERROR, the descriptor is : %p\n", descriptor);
        goto FAILED;
    }
    if (index <= 0) {
        goto FAILED;
    }
    // find the obj via the id;
    struct descriptor__link_list_t* d = descriptor->data;
    while (d) {
        if (d->index == index) {
            break;
        }
        d = d->next;
    }
    if (!d) {   // cannot find that index
        goto FAILED;
    }
SUCCEED:
    *obj_size = d->obj_size;
    return d->obj_ptr;
FAILED:
    *obj_size = 0;
    return NULL;
}

