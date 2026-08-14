#include <stdint.h>
#include <string.h>

#include <ce/deque.h>
#include <ce/mem.h>

static CeResult ce_deque_reserve(CeDeque *deque, size_t capacity)
{
    size_t bytes;
    size_t old_bytes;
    unsigned char *data;
    CeResult result;

    if (capacity <= deque->cap)
        return CE_OK;

    result = ce_size_mul(capacity, deque->elem_size, &bytes);
    if (result != CE_OK)
        return result;

    data = ce_mem_alloc(bytes);
    if (data == NULL)
        return CE_OUT_OF_MEMORY;

    result = ce_size_mul(deque->len, deque->elem_size, &old_bytes);
    if (result != CE_OK) {
        ce_mem_free(data);
        return result;
    }

    if (deque->len != 0) {
        size_t first = (deque->cap - deque->head) * deque->elem_size;

        if (first >= old_bytes) {
            memcpy(data, deque->data + deque->head * deque->elem_size, old_bytes);
        } else {
            memcpy(data, deque->data + deque->head * deque->elem_size, first);
            memcpy(data + first, deque->data, old_bytes - first);
        }
    }

    ce_mem_free(deque->data);
    deque->data = data;
    deque->cap = capacity;
    deque->head = 0;
    return CE_OK;
}

static CeResult ce_deque_grow(CeDeque *deque)
{
    size_t capacity = deque->cap == 0 ? 8 : deque->cap;

    if (deque->cap == 0)
        return ce_deque_reserve(deque, capacity);
    if (capacity > SIZE_MAX / 2)
        return CE_OVERFLOW;

    return ce_deque_reserve(deque, capacity * 2);
}

CeResult ce_deque_init(CeDeque *deque, size_t elem_size)
{
    if (deque == NULL || elem_size == 0)
        return CE_INVALID_ARGUMENT;

    memset(deque, 0, sizeof(*deque));
    deque->elem_size = elem_size;
    return CE_OK;
}

void ce_deque_free(CeDeque *deque)
{
    if (deque == NULL)
        return;

    ce_mem_free(deque->data);
    memset(deque, 0, sizeof(*deque));
}

void ce_deque_clear(CeDeque *deque)
{
    if (deque == NULL)
        return;

    deque->len = 0;
    deque->head = 0;
}

CeResult ce_deque_push_back(CeDeque *deque, const void *value)
{
    size_t needed;
    size_t index;
    CeResult result;

    if (deque == NULL || deque->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_size_add(deque->len, 1, &needed);
    if (result != CE_OK)
        return result;
    if (needed > deque->cap) {
        result = ce_deque_grow(deque);
        if (result != CE_OK)
            return result;
    }

    index = deque->len < deque->cap - deque->head ? deque->head + deque->len
                                                   : deque->len - (deque->cap - deque->head);
    memcpy(deque->data + index * deque->elem_size, value, deque->elem_size);
    deque->len = needed;
    return CE_OK;
}

CeResult ce_deque_push_front(CeDeque *deque, const void *value)
{
    size_t needed;
    size_t index;
    CeResult result;

    if (deque == NULL || deque->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_size_add(deque->len, 1, &needed);
    if (result != CE_OK)
        return result;
    if (needed > deque->cap) {
        result = ce_deque_grow(deque);
        if (result != CE_OK)
            return result;
    }

    index = deque->head == 0 ? deque->cap - 1 : deque->head - 1;
    memcpy(deque->data + index * deque->elem_size, value, deque->elem_size);
    deque->head = index;
    deque->len = needed;
    return CE_OK;
}

CeResult ce_deque_pop_front(CeDeque *deque, void *value)
{
    if (deque == NULL || deque->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;
    if (deque->len == 0)
        return CE_INVALID_DATA;

    memcpy(value, deque->data + deque->head * deque->elem_size, deque->elem_size);
    deque->head = (deque->head + 1) % deque->cap;
    deque->len -= 1;
    if (deque->len == 0)
        deque->head = 0;
    return CE_OK;
}

CeResult ce_deque_pop_back(CeDeque *deque, void *value)
{
    size_t index;

    if (deque == NULL || deque->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;
    if (deque->len == 0)
        return CE_INVALID_DATA;

    index = deque->len - 1;
    index = index < deque->cap - deque->head ? deque->head + index
                                              : index - (deque->cap - deque->head);
    memcpy(value, deque->data + index * deque->elem_size, deque->elem_size);
    deque->len -= 1;
    if (deque->len == 0)
        deque->head = 0;
    return CE_OK;
}

unsigned char *ce_deque_data(CeDeque *deque)
{
    return deque == NULL ? NULL : deque->data;
}

const unsigned char *ce_deque_const_data(const CeDeque *deque)
{
    return deque == NULL ? NULL : deque->data;
}

size_t ce_deque_len(const CeDeque *deque)
{
    return deque == NULL ? 0 : deque->len;
}

size_t ce_deque_capacity(const CeDeque *deque)
{
    return deque == NULL ? 0 : deque->cap;
}

size_t ce_deque_elem_size(const CeDeque *deque)
{
    return deque == NULL ? 0 : deque->elem_size;
}
