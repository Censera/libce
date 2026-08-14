#ifndef CE_H
#define CE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>

void *ce_alloc(size_t size);
void *ce_calloc(size_t count, size_t size);
void *ce_realloc(void *ptr, size_t size);
void ce_free(void *ptr);

int ce_size_add(size_t a, size_t b, size_t *out);
int ce_size_mul(size_t a, size_t b, size_t *out);

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} ce_buffer;

void ce_buffer_init(ce_buffer *buffer);
void ce_buffer_free(ce_buffer *buffer);
void ce_buffer_clear(ce_buffer *buffer);
int ce_buffer_reserve(ce_buffer *buffer, size_t capacity);
int ce_buffer_append(ce_buffer *buffer, const void *data, size_t size);

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} ce_string;

void ce_string_init(ce_string *string);
void ce_string_free(ce_string *string);
int ce_string_set(ce_string *string, const char *value);
int ce_string_append(ce_string *string, const char *value);

int ce_file_read(const char *path, ce_buffer *buffer);
int ce_file_write(const char *path, const void *data, size_t size);

#endif
