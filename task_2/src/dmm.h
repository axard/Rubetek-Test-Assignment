#ifndef DMM_H
#define DMM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


void* dmm_malloc(size_t size);

void dmm_free(void *ptr);

size_t dmm_avail(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DMM_H */
