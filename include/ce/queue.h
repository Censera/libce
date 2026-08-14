#ifndef CE_QUEUE_H
#define CE_QUEUE_H

#include <ce/buf.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeQueue {
    CeBuf storage;
    size_t head;
    size_t elem_size;
} CeQueue;

CeResult ce_queue_init(CeQueue *queue, size_t elem_size);
void ce_queue_free(CeQueue *queue);
void ce_queue_clear(CeQueue *queue);
CeResult ce_queue_push(CeQueue *queue, const void *value);
CeResult ce_queue_pop(CeQueue *queue, void *value);
size_t ce_queue_len(const CeQueue *queue);
int ce_queue_empty(const CeQueue *queue);

#ifdef __cplusplus
}
#endif

#endif
