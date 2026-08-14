#ifndef CE_DEQUE_H
#define CE_DEQUE_H

#include <stddef.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeDeque {
    unsigned char *data;
    size_t len;
    size_t cap;
    size_t head;
    size_t elem_size;
} CeDeque;

CeResult ce_deque_init(CeDeque *deque, size_t elem_size);
void ce_deque_free(CeDeque *deque);
void ce_deque_clear(CeDeque *deque);

CeResult ce_deque_push_front(CeDeque *deque, const void *value);
CeResult ce_deque_push_back(CeDeque *deque, const void *value);
CeResult ce_deque_pop_front(CeDeque *deque, void *value);
CeResult ce_deque_pop_back(CeDeque *deque, void *value);

unsigned char *ce_deque_data(CeDeque *deque);
const unsigned char *ce_deque_const_data(const CeDeque *deque);
size_t ce_deque_len(const CeDeque *deque);
size_t ce_deque_capacity(const CeDeque *deque);
size_t ce_deque_elem_size(const CeDeque *deque);

#ifdef __cplusplus
}
#endif

#endif
