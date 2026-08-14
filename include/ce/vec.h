#ifndef CE_VEC_H
#define CE_VEC_H

#include <stddef.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeVec {
    unsigned char *data;
    size_t len;
    size_t cap;
    size_t elem_size;
} CeVec;

CeResult ce_vec_init(CeVec *vec, size_t elem_size);
void ce_vec_free(CeVec *vec);
void ce_vec_clear(CeVec *vec);

CeResult ce_vec_reserve(CeVec *vec, size_t capacity);
CeResult ce_vec_resize(CeVec *vec, size_t len);

/* value must not point into vec. */
CeResult ce_vec_push(CeVec *vec, const void *value);
CeResult ce_vec_pop(CeVec *vec, void *value);

unsigned char *ce_vec_data(CeVec *vec);
const unsigned char *ce_vec_const_data(const CeVec *vec);
size_t ce_vec_len(const CeVec *vec);
size_t ce_vec_capacity(const CeVec *vec);
size_t ce_vec_elem_size(const CeVec *vec);

#ifdef __cplusplus
}
#endif

#endif
