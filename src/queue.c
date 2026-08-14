#include <string.h>

#include <ce/mem.h>
#include <ce/queue.h>

CeResult ce_queue_init(CeQueue *queue, size_t elem_size)
{
    if (queue == NULL || elem_size == 0)
        return CE_INVALID_ARGUMENT;
    ce_buf_init(&queue->storage);
    queue->head = 0;
    queue->elem_size = elem_size;
    return CE_OK;
}

void ce_queue_free(CeQueue *queue)
{
    if (queue == NULL)
        return;
    ce_buf_free(&queue->storage);
    queue->head = 0;
    queue->elem_size = 0;
}

void ce_queue_clear(CeQueue *queue)
{
    if (queue == NULL)
        return;
    ce_buf_clear(&queue->storage);
    queue->head = 0;
}

CeResult ce_queue_push(CeQueue *queue, const void *value)
{
    if (queue == NULL || queue->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;
    return ce_buf_append(&queue->storage, value, queue->elem_size);
}

CeResult ce_queue_pop(CeQueue *queue, void *value)
{
    size_t count;
    size_t offset;

    if (queue == NULL || queue->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;
    if (queue->storage.len < queue->head)
        return CE_INVALID_DATA;

    count = (queue->storage.len - queue->head) / queue->elem_size;
    if (count == 0)
        return CE_INVALID_DATA;

    offset = queue->head;
    memcpy(value, queue->storage.data + offset, queue->elem_size);
    queue->head += queue->elem_size;

    if (queue->head == queue->storage.len) {
        ce_buf_clear(&queue->storage);
        queue->head = 0;
    } else if (queue->head >= queue->storage.cap / 2) {
        size_t remaining = queue->storage.len - queue->head;
        memmove(queue->storage.data, queue->storage.data + queue->head, remaining);
        queue->storage.len = remaining;
        queue->head = 0;
    }

    return CE_OK;
}

size_t ce_queue_len(const CeQueue *queue)
{
    if (queue == NULL || queue->elem_size == 0 || queue->storage.len < queue->head)
        return 0;
    return (queue->storage.len - queue->head) / queue->elem_size;
}

int ce_queue_empty(const CeQueue *queue)
{
    return ce_queue_len(queue) == 0;
}
