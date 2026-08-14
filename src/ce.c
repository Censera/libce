#include "ce.h"

#include <stdlib.h>
#include <string.h>

void *ce_alloc(size_t size)
{
    return malloc(size);
}

void *ce_calloc(size_t count, size_t size)
{
    return calloc(count, size);
}

void *ce_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

void ce_free(void *ptr)
{
    free(ptr);
}

int ce_size_add(size_t a, size_t b, size_t *out)
{
    if (a > SIZE_MAX - b)
        return 0;
    *out = a + b;
    return 1;
}

int ce_size_mul(size_t a, size_t b, size_t *out)
{
    if (a != 0 && b > SIZE_MAX / a)
        return 0;
    *out = a * b;
    return 1;
}

void ce_buffer_init(ce_buffer *buffer)
{
    buffer->data = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

void ce_buffer_free(ce_buffer *buffer)
{
    free(buffer->data);
    ce_buffer_init(buffer);
}

void ce_buffer_clear(ce_buffer *buffer)
{
    buffer->size = 0;
}

int ce_buffer_reserve(ce_buffer *buffer, size_t capacity)
{
    unsigned char *data;

    if (capacity <= buffer->capacity)
        return 1;

    data = realloc(buffer->data, capacity);
    if (data == NULL)
        return 0;

    buffer->data = data;
    buffer->capacity = capacity;
    return 1;
}

int ce_buffer_append(ce_buffer *buffer, const void *data, size_t size)
{
    size_t required;
    size_t capacity;

    if (!ce_size_add(buffer->size, size, &required))
        return 0;
    if (required == 0)
        return 1;
    if (required <= buffer->capacity)
        goto copy;

    capacity = buffer->capacity ? buffer->capacity : 64;
    while (capacity < required) {
        size_t next;
        if (!ce_size_mul(capacity, 2, &next)) {
            capacity = required;
            break;
        }
        capacity = next;
    }

    if (!ce_buffer_reserve(buffer, capacity))
        return 0;

copy:
    memcpy(buffer->data + buffer->size, data, size);
    buffer->size = required;
    return 1;
}

void ce_string_init(ce_string *string)
{
    string->data = NULL;
    string->length = 0;
    string->capacity = 0;
}

void ce_string_free(ce_string *string)
{
    free(string->data);
    ce_string_init(string);
}

int ce_string_set(ce_string *string, const char *value)
{
    size_t length = strlen(value);
    char *data;

    if (length == SIZE_MAX)
        return 0;

    data = realloc(string->data, length + 1);
    if (data == NULL)
        return 0;

    memcpy(data, value, length + 1);
    string->data = data;
    string->length = length;
    string->capacity = length + 1;
    return 1;
}

int ce_string_append(ce_string *string, const char *value)
{
    size_t length = strlen(value);
    size_t total;
    char *data;

    if (!ce_size_add(string->length, length, &total) || total == SIZE_MAX)
        return 0;

    data = realloc(string->data, total + 1);
    if (data == NULL)
        return 0;

    memcpy(data + string->length, value, length + 1);
    string->data = data;
    string->length = total;
    string->capacity = total + 1;
    return 1;
}

int ce_file_read(const char *path, ce_buffer *buffer)
{
    FILE *file;
    unsigned char chunk[8192];
    size_t count;

    file = fopen(path, "rb");
    if (file == NULL)
        return 0;

    ce_buffer_clear(buffer);
    while ((count = fread(chunk, 1, sizeof(chunk), file)) != 0) {
        if (!ce_buffer_append(buffer, chunk, count)) {
            fclose(file);
            return 0;
        }
    }

    if (ferror(file)) {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int ce_file_write(const char *path, const void *data, size_t size)
{
    FILE *file;
    size_t written;

    file = fopen(path, "wb");
    if (file == NULL)
        return 0;

    written = fwrite(data, 1, size, file);
    if (written != size || fclose(file) != 0)
        return 0;

    return 1;
}
