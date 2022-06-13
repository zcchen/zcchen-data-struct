#ifndef ZCCHEN_OBJECT_H
#define ZCCHEN_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif  /* ifdef __cplusplus */

#include <stdint.h>
// the method to create the data object
typedef void*
(*obj_create_func)(uint32_t* allocate_size, void* param_data, uint32_t param_size, void* context);

// the method to destroy the data object
typedef void
(*obj_destroy_func)(void** object, void* context);

#ifdef __cplusplus
}
#endif  /* ifdef __cplusplus */



#endif /* ifndef ZCCHEN_OBJECT_H */


