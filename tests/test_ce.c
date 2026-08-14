#include "ce.h"

#include <assert.h>
#include <string.h>

static void test_size(void)
{
    size_t value;
    assert(ce_size_add(2, 3, &value));
    assert(value == 5);
    assert(!ce_size_add(SIZE_MAX, 1, &value));
    assert(ce_size_mul(4, 5, &value));
    assert(value == 20);
    assert(!ce_size_mul(SIZE_MAX, 2, &value));
}

static void test_buffer(void)
{
    ce_buffer buffer;
    const char text[] = "hello";

    ce_buffer_init(&buffer);
    assert(ce_buffer_append(&buffer, text, sizeof(text) - 1));
    assert(buffer.size == 5);
    assert(memcmp(buffer.data, text, 5) == 0);
    ce_buffer_clear(&buffer);
    assert(buffer.size == 0);
    ce_buffer_free(&buffer);
    assert(buffer.data == NULL);
    assert(buffer.capacity == 0);
}

static void test_string(void)
{
    ce_string string;

    ce_string_init(&string);
    assert(ce_string_set(&string, "hello"));
    assert(ce_string_append(&string, " world"));
    assert(strcmp(string.data, "hello world") == 0);
    assert(string.length == 11);
    ce_string_free(&string);
    assert(string.data == NULL);
}

int main(void)
{
    test_size();
    test_buffer();
    test_string();
    return 0;
}
