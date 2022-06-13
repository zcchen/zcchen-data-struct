#ifndef ZCCHEN_DESCRIPTOR_H
#define ZCCHEN_DESCRIPTOR_H

#include "zcchen/config-dll.h"
#include "zcchen/object.h"

#ifdef __cplusplus
extern "C" {
#endif  /* ifdef __cplusplus */

#include <stdint.h>

// internal data struct
struct zcchen_descriptor_t;

CONFIGAPI struct zcchen_descriptor_t* CONFIGCALL
zcchen_descriptor_create(int32_t max_size, obj_create_func create_func, obj_destroy_func destroy_func);

CONFIGAPI void CONFIGCALL
zcchen_descriptor_destroy(struct zcchen_descriptor_t **descriptor);

// return int (created id),
// > 0 if succeed
// <= 0 means error
CONFIGAPI int32_t CONFIGCALL
zcchen_descriptor_add(struct zcchen_descriptor_t* descriptor, void* param_data, uint32_t param_size);

// == 0 means succeed.
// == 1 maens not found.
CONFIGAPI int CONFIGCALL
zcchen_descriptor_remove(struct zcchen_descriptor_t* descriptor, int32_t index);

CONFIGAPI void* CONFIGCALL
zcchen_descriptor_getObj(struct zcchen_descriptor_t* descriptor, int32_t index, uint32_t* obj_size);


#ifdef __cplusplus
}
#endif  /* ifdef __cplusplus */

#endif /* ifndef ZCCHEN_DESCRIPTOR_H */
